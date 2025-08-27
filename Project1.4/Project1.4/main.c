// =========================================================================
// 파일명: main.c
// 기능: 비밀번호 잠금장치 시스템의 메인 로직을 구현합니다.
//       - 비밀번호 입력, 정답/오답 판별
//       - 관리자 비밀번호를 통한 관리자 모드 진입
//       - 새로운 비밀번호 설정 기능
// =========================================================================

#define F_CPU 14745600UL // CPU 클럭 주파수 정의 (Hz). _delay_ms/_delay_us 함수 사용에 필수적입니다.
                         // 이 값은 사용하는 ATmega128 보드의 실제 클럭과 일치해야 합니다.

#include <avr/io.h>         // AVR 입출력 레지스터 정의 (PORT, DDR, PIN 등)
#include <avr/interrupt.h>  // 인터럽트 관련 함수 정의 (sei() 등)
#include <util/delay.h>     // 딜레이 함수 (_delay_ms/_delay_us) 사용
#include <string.h>         // 문자열 처리 함수 (strcmp, memset, strcpy) 사용

#include "lcd/lcd.h"            // LCD 제어 라이브러리 헤더 파일
#include "keypad/keypad.h"         // 키패드 제어 라이브러리 헤더 파일
#include "led/led.h"            // 풀컬러 LED 제어 라이브러리 헤더 파일


// =========================================================================
// 1. 전역 상수 및 타입 정의
// =========================================================================

#define MAX_PASSWORD_LENGTH 7 // 일반 비밀번호 및 새 비밀번호의 최대 길이 (7자리)

// 프로그램의 현재 상태를 나타내는 열거형 (Enum)
typedef enum {
    PROGRAM_STATE_INPUT_PASSWORD,   // 0: 비밀번호 입력 대기 상태
    PROGRAM_STATE_ADMIN_MODE,       // 1: 관리자 모드 (비밀번호 변경 옵션 제공)
    PROGRAM_STATE_CHANGE_PASSWORD   // 2: 새로운 비밀번호 입력 중인 상태
} ProgramState_t;

// 관리자 비밀번호 (5자리, 코드에 고정되어 변경되지 않음)
#define ADMIN_PASSWORD "98765"
#define ADMIN_PASSWORD_LENGTH 5 // 관리자 비밀번호의 길이


// =========================================================================
// 2. 전역 변수 선언
// =========================================================================

// 현재 프로그램 상태를 저장하는 변수
volatile ProgramState_t current_program_state = PROGRAM_STATE_INPUT_PASSWORD;

// 현재 시스템에 저장된 정답 비밀번호. (volatile: 최적화 방지, 컴파일러에게 값이 외부에서 변경될 수 있음을 알림)
// NOTE: 실제 제품에서는 전원이 꺼져도 유지되도록 EEPROM에 저장해야 합니다.
volatile char stored_password[MAX_PASSWORD_LENGTH + 1] = "1234567"; // 초기 비밀번호는 "1234567"

// 사용자가 키패드로 입력한 비밀번호를 저장하는 버퍼 (널 종료 문자 '\0'를 위해 +1 크기)
char entered_password[MAX_PASSWORD_LENGTH + 1];

// entered_password 버퍼에 현재까지 입력된 문자의 개수를 추적하는 인덱스
int password_index = 0;


// =========================================================================
// 3. 함수 선언 (프로토타입은 일반적으로 헤더 파일에 있지만, main에서만 쓰는 보조 함수는 여기에)
// =========================================================================

// 프로그램의 초기 상태로 되돌리는 함수
void reset_program(void);

// 관리자 모드로 진입 시 LCD 메시지 및 상태 설정을 담당하는 함수
void enter_admin_mode(void);

// 비밀번호 변경 모드로 진입 시 LCD 메시지 및 상태 설정을 담당하는 함수
void enter_change_password_mode(void);


// =========================================================================
// 4. 함수 구현
// =========================================================================

/**
 * @brief 프로그램을 초기 상태로 되돌리고 LCD 메시지를 초기화합니다.
 */
void reset_program(void) {
    LCD_Clear();                                    // LCD 화면을 지웁니다.
    LCD_pos(0, 0);                                  // 커서를 첫 번째 줄(row 0), 첫 번째 칸(col 0)으로 이동합니다.
    LCD_STR((unsigned char*)"Input PassWord");       // "Input PassWord" 메시지를 첫 번째 줄에 출력합니다.
    
    current_program_state = PROGRAM_STATE_INPUT_PASSWORD; // 프로그램 상태를 비밀번호 입력 모드로 설정합니다.
    
    LCD_pos(0, 1);                                  // 숫자 입력을 위한 커서를 두 번째 줄(row 1), 첫 번째 칸(col 0)으로 이동합니다.
    
    password_index = 0;                             // 입력된 비밀번호 인덱스를 0으로 초기화합니다.
    memset(entered_password, 0, sizeof(entered_password)); // entered_password 버퍼를 모두 0으로 지웁니다.
    led_off();                                      // 풀컬러 LED를 모두 끕니다.
}

/**
 * @brief 관리자 모드로 진입 시 LCD 메시지 및 상태를 설정합니다.
 */
void enter_admin_mode(void) {
    LCD_Clear();                                    // LCD 화면을 지웁니다.
    LCD_pos(0, 0);                                  // 커서를 첫 번째 줄(row 0), 첫 번째 칸(col 0)으로 이동합니다.
    LCD_STR((unsigned char*)"Admin Mode");          // "Admin Mode" 메시지를 출력합니다.
    LCD_pos(0, 1);                                  // 커서를 두 번째 줄(row 1), 첫 번째 칸(col 0)으로 이동합니다.
    LCD_STR((unsigned char*)"# for New PWD");       // "# for New PWD" 안내 메시지를 출력합니다.

    current_program_state = PROGRAM_STATE_ADMIN_MODE; // 프로그램 상태를 관리자 모드로 설정합니다.
    
    // 이 모드에서는 비밀번호를 입력받지 않으므로, 입력 버퍼와 인덱스는 초기화 상태로 둡니다.
    password_index = 0;                             
    memset(entered_password, 0, sizeof(entered_password)); 
}

/**
 * @brief 새 비밀번호 설정 모드로 진입 시 LCD 메시지 및 상태를 설정합니다.
 */
void enter_change_password_mode(void) {
    LCD_Clear();                                    // LCD 화면을 지웁니다.
    LCD_pos(0, 0);                                  // 커서를 첫 번째 줄(row 0), 첫 번째 칸(col 0)으로 이동합니다.
    LCD_STR((unsigned char*)"Enter New PWD");       // "Enter New PWD" 메시지를 출력합니다.
    LCD_pos(0, 1);                                  // 새 비밀번호 입력을 위한 커서를 두 번째 줄(row 1), 첫 번째 칸(col 0)으로 이동합니다.
    
    current_program_state = PROGRAM_STATE_CHANGE_PASSWORD; // 프로그램 상태를 비밀번호 변경 모드로 설정합니다.
    
    password_index = 0;                             // 입력된 비밀번호 인덱스를 0으로 초기화합니다.
    memset(entered_password, 0, sizeof(entered_password)); // entered_password 버퍼를 지웁니다.
}

/**
 * @brief 메인 함수: 프로그램의 시작점이며 무한 루프를 통해 시스템을 운영합니다.
 */
int main(void) {
    // 하드웨어 포트 방향 설정 (각 모듈의 초기화 함수에서 대부분 처리됩니다.)
    // LED (PORTE)는 main에서 직접 DDR 설정
    DDRE = 0x07; // PORTE의 PE0, PE1, PE2 핀을 출력으로 설정합니다 (풀컬러 LED 제어용).

    // 각 모듈 (LCD, 키패드, LED) 초기화
    Port_Init();    // LCD 포트 초기화 (lcd.c에 정의되어 있음)
    LCD_Init();     // LCD 컨트롤러 초기화 (lcd.c에 정의되어 있음)
    Keypad_Init();  // 키패드 포트 초기화 (keypad.c에 정의되어 있음)
    led_init();     // 풀컬러 LED 초기화 (led.c에 정의되어 있음)

    sei(); // Global Interrupt Enable (필요하다면 활성화, 여기서는 폴링 방식이므로 필수 아님)

    reset_program(); // 프로그램 시작 시 초기 상태로 설정합니다.

    // 메인 무한 루프
    while (1) {
        char key = keypad_get_char(); // 키패드에서 눌린 키 값을 읽어옵니다. (없으면 '\0' 반환)

        if (key != '\0') { // 키가 입력되었다면
            // 키 디바운싱: 키 눌림이 여러 번 감지되는 것을 방지합니다.
            _delay_ms(50); // 키 눌림 감지 후 짧게 대기하여 채터링(chattering)을 무시합니다.
            while (keypad_get_char() != '\0'); // 키가 떼어질 때까지 계속 키패드를 읽어 대기합니다.
            _delay_ms(50); // 키가 완전히 떼어진 것을 확인 후 짧게 대기합니다.

            // 현재 프로그램 상태에 따라 다른 동작을 수행합니다 (상태 머신).
            switch (current_program_state) {
                case PROGRAM_STATE_INPUT_PASSWORD:
                    // '비밀번호 입력 대기' 상태의 로직
                    if (key == '*') { // '*' 키는 마지막 입력 지우기 기능으로 사용합니다.
                        if (password_index > 0) { // 입력된 문자가 있다면
                            password_index--;                      // 인덱스를 줄입니다.
                            entered_password[password_index] = '\0'; // 해당 위치의 문자를 지웁니다.
                            LCD_pos(password_index, 1);            // LCD 커서를 지울 위치로 이동합니다.
                            LCD_CHAR(' ');                         // 공백을 출력하여 문자를 지운 것처럼 보입니다.
                            LCD_pos(password_index, 1);            // 커서를 다시 지운 위치로 돌려놓습니다.
                        }
                    } else if (key == '#') { // '#' 키는 입력 완료 기능으로 사용합니다.
                        // 사용자가 입력한 비밀번호의 길이에 따라 일반 비밀번호 또는 관리자 비밀번호를 확인합니다.
                        if (password_index == MAX_PASSWORD_LENGTH) { // 입력된 길이가 일반 비밀번호 길이(7자리)와 같다면
                            // 입력된 비밀번호와 저장된 비밀번호(stored_password)를 비교합니다.
                            if (strcmp(entered_password, (char*)stored_password) == 0) { // 비밀번호가 일치한다면 (정답)
                                LCD_Clear();                                // LCD를 지웁니다.
                                LCD_pos(0, 0);                              // 커서를 첫 줄로 이동합니다.
                                LCD_STR((unsigned char*)"OPEN");            // "OPEN" 메시지를 출력합니다.
                                led_set_color(LED_GREEN);                   // 풀컬러 LED를 초록색으로 켭니다.
                                _delay_ms(5000);                            // 5초 동안 유지합니다.
                                led_set_color(LED_YELLOW);                  // LED를 노란색으로 변경합니다.
                                _delay_ms(1000);                            // 1초 동안 유지합니다 (다음 루프를 위해 잠시).
                                reset_program();                            // 프로그램 상태를 초기화합니다.
                            } else { // 비밀번호가 일치하지 않는다면 (오답)
                                LCD_Clear();                                // LCD를 지웁니다.
                                LCD_pos(0, 0);                              // 커서를 첫 줄로 이동합니다.
                                LCD_STR((unsigned char*)"Not PassWord");    // "Not PassWord" 메시지를 출력합니다.
                                led_set_color(LED_RED);                     // 풀컬러 LED를 빨간색으로 켭니다.
                                _delay_ms(2000);                            // 2초 동안 유지합니다.
                                led_set_color(LED_YELLOW);                  // LED를 노란색으로 변경합니다.
                                _delay_ms(1000);                            // 1초 동안 유지합니다.
                                reset_program();                            // 프로그램 상태를 초기화합니다.
                            }
                        } 
                        // 입력된 길이가 관리자 비밀번호 길이(5자리)와 같다면
                        else if (password_index == ADMIN_PASSWORD_LENGTH) {
                             // 입력된 비밀번호와 관리자 비밀번호(ADMIN_PASSWORD)를 비교합니다.
                             if (strcmp(entered_password, ADMIN_PASSWORD) == 0) { // 관리자 비밀번호가 일치한다면
                                enter_admin_mode();                             // 관리자 모드로 진입합니다.
                            } else { // 관리자 비밀번호가 일치하지 않는다면
                                LCD_Clear();                                    // LCD를 지웁니다.
                                LCD_pos(0, 0);                                  // 커서를 첫 줄로 이동합니다.
                                LCD_STR((unsigned char*)"Not Admin PWD");       // "Not Admin PWD" 메시지를 출력합니다.
                                led_set_color(LED_RED);                         // 풀컬러 LED를 빨간색으로 켭니다.
                                _delay_ms(2000);                                // 2초 동안 유지합니다.
                                led_set_color(LED_YELLOW);                      // LED를 노란색으로 변경합니다.
                                _delay_ms(1000);                                // 1초 동안 유지합니다.
                                reset_program();                                // 프로그램 상태를 초기화합니다.
                            }
                        }
                        else { // 비밀번호 길이가 조건에 맞지 않을 때 (7자리 또는 5자리 요구)
                             LCD_pos(0, 1);                                    // 커서를 두 번째 줄로 이동합니다.
                             LCD_STR((unsigned char*)"         ");             // 기존 입력 내용을 지우기 위해 공백을 출력합니다.
                             LCD_pos(0, 1);                                    // 커서를 다시 두 번째 줄 시작 위치로 이동합니다.
                             LCD_STR((unsigned char*)"7 or 5 digits");         // "7 or 5 digits" 안내 메시지를 출력합니다.
                             _delay_ms(1000);                                  // 1초 동안 메시지를 보여줍니다.
                             LCD_pos(0, 1);                                    // 커서를 다시 입력 위치로 이동합니다.
                             // 이전에 입력된 숫자를 다시 표시하여 사용자가 이어서 입력할 수 있도록 합니다.
                             for(int i=0; i<password_index; i++) {
                                LCD_CHAR((unsigned char)entered_password[i]);
                             }
                        }
                    } else { // 숫자 키가 입력되었을 때
                        if (password_index < MAX_PASSWORD_LENGTH) { // 현재 입력 길이가 최대 길이(7자리) 미만인 경우에만 입력 받습니다.
                            entered_password[password_index] = key;  // 입력된 키를 버퍼에 저장합니다.
                            LCD_CHAR((unsigned char)key);            // LCD 두 번째 줄에 해당 숫자를 출력합니다.
                            password_index++;                        // 인덱스를 증가시킵니다.
                            entered_password[password_index] = '\0'; // 다음 입력을 위해 널 종료 문자를 추가합니다.
                        }
                    }
                    break; // PROGRAM_STATE_INPUT_PASSWORD 케이스 종료

                case PROGRAM_STATE_ADMIN_MODE:
                    // '관리자 모드' 상태의 로직
                    if (key == '#') { // '#' 키를 누르면 '새 비밀번호 설정' 모드로 진입합니다.
                        enter_change_password_mode();
                    } else if (key == '*') { // '*' 키를 누르면 관리자 모드를 종료하고 초기 상태로 돌아갑니다.
                        reset_program();
                    } else { // 그 외의 키가 입력되면 잘못된 키임을 알립니다.
                        LCD_pos(0, 1);                             // 커서를 두 번째 줄로 이동합니다.
                        LCD_STR((unsigned char*)"Invalid Key");   // "Invalid Key" 메시지를 출력합니다.
                        _delay_ms(1000);                           // 1초 동안 메시지를 보여줍니다.
                        LCD_pos(0, 1);                             // 커서를 다시 두 번째 줄 시작 위치로 이동합니다.
                        LCD_STR((unsigned char*)"# for New PWD"); // 원래 안내 메시지를 다시 출력합니다.
                    }
                    break; // PROGRAM_STATE_ADMIN_MODE 케이스 종료

                case PROGRAM_STATE_CHANGE_PASSWORD:
                    // '새 비밀번호 설정' 상태의 로직
                    if (key == '*') { // '*' 키를 누르면 비밀번호 변경을 취소하고 초기 상태로 돌아갑니다.
                        reset_program();
                    } else if (key == '#') { // '#' 키를 누르면 새 비밀번호 입력을 완료합니다.
                        if (password_index == MAX_PASSWORD_LENGTH) { // 새 비밀번호가 7자리로 모두 입력되었다면
                            // entered_password에 있는 새 비밀번호를 stored_password로 복사합니다.
                            strcpy((char*)stored_password, entered_password); 
                            LCD_Clear();                                // LCD를 지웁니다.
                            LCD_pos(0, 0);                              // 커서를 첫 줄로 이동합니다.
                            LCD_STR((unsigned char*)"PWD Changed!");    // "PWD Changed!" 메시지를 출력합니다.
                            led_set_color(LED_GREEN);                   // 풀컬러 LED를 초록색으로 켭니다.
                            _delay_ms(3000);                            // 3초 동안 유지합니다.
                            led_set_color(LED_YELLOW);                  // LED를 노란색으로 변경합니다.
                            _delay_ms(1000);                            // 1초 동안 유지합니다.
                            reset_program();                            // 프로그램 상태를 초기화합니다.
                        } else { // 7자리가 채워지지 않았을 때
                            LCD_pos(0, 1);                             // 커서를 두 번째 줄로 이동합니다.
                            LCD_STR((unsigned char*)"         ");       // 기존 입력 내용을 지우기 위해 공백을 출력합니다.
                            LCD_pos(0, 1);                             // 커서를 다시 두 번째 줄 시작 위치로 이동합니다.
                            LCD_STR((unsigned char*)"7 digits Req");  // "7 digits Req" 안내 메시지를 출력합니다.
                            _delay_ms(1000);                           // 1초 동안 메시지를 보여줍니다.
                            LCD_pos(0, 1);                             // 커서를 다시 입력 위치로 이동합니다.
                            // 이전에 입력된 숫자를 다시 표시하여 사용자가 이어서 입력할 수 있도록 합니다.
                            for(int i=0; i<password_index; i++) {
                                LCD_CHAR((unsigned char)entered_password[i]);
                            }
                        }
                    } else { // 숫자 키가 입력되었을 때
                        if (password_index < MAX_PASSWORD_LENGTH) { // 현재 입력 길이가 최대 길이(7자리) 미만인 경우에만 입력 받습니다.
                            entered_password[password_index] = key;  // 입력된 키를 버퍼에 저장합니다.
                            LCD_CHAR((unsigned char)key);            // LCD 두 번째 줄에 해당 숫자를 출력합니다.
                            password_index++;                        // 인덱스를 증가시킵니다.
                            entered_password[password_index] = '\0'; // 다음 입력을 위해 널 종료 문자를 추가합니다.
                        }
                    }
                    break; // PROGRAM_STATE_CHANGE_PASSWORD 케이스 종료
            }
        }
    }
}