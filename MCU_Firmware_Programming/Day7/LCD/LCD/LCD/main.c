/*
 * ATmega128 + HD44780 LCD 16x2 제어 예제
 * ----------------------------------------
 * 하드웨어 연결
 * ----------------------------------------
 * 데이터 핀 D0~D7 : ATmega128 PORTB (PB0 ~ PB7)
 * 제어 핀 RS, RW, EN : ATmega128 PORTG (PG0, PG1, PG2)
 * 
 * LCD 전원 및 접지 필수 연결:
 *  - VSS : GND
 *  - VDD : +5V
 *  - V0  : 가변저항(10kΩ) 중앙, 나머지 양 끝단은 +5V, GND에 연결 (화면 콘트라스트 조절)
 *  - A, K : 백라이트 (+5V, GND)
 *
 * 이 코드는 8비트 모드로 LCD를 제어합니다.
 * 
 * 작성자: ChatGPT
 * 작성일: 2025-08-13
 */

#define F_CPU 16000000UL    // CPU 클럭 주파수 16MHz를 정의 (delay 함수 동작에 필요)
#include <avr/io.h>         // AVR 입출력 포트 정의 헤더
#include <util/delay.h>     // 지연 함수 (_delay_ms, _delay_us)

// LCD 제어 핀(PORTG 사용)
#define LCD_RS PG0          // RS 핀: 0=명령어, 1=데이터 선택
#define LCD_RW PG1          // RW 핀: 0=쓰기, 1=읽기 선택 (대부분 0으로 사용)
#define LCD_EN PG2          // EN 핀: 데이터 읽기/쓰기 활성화 신호 (Enable)

// 제어 포트 방향 및 출력 포트
#define LCD_CTRL_DDR DDRG   // 제어 핀 포트 방향 레지스터
#define LCD_CTRL_PORT PORTG // 제어 핀 출력 레지스터

// LCD 데이터 핀(PORTB 사용)
#define LCD_DATA_DDR DDRB   // 데이터 핀 포트 방향 레지스터
#define LCD_DATA_PORT PORTB // 데이터 핀 출력 레지스터

// LCD 명령어 상수 (HD44780 데이터 시트 참고)
#define LCD_CLEAR 0x01           // 화면 클리어 명령어
#define LCD_RETURN_HOME 0x02     // 커서 위치 0,0으로 복귀
#define LCD_ENTRY_MODE 0x06      // 문자 입력시 커서 오른쪽 이동, 화면 이동 없음
#define LCD_DISPLAY_ON 0x0C      // 디스플레이 ON, 커서 OFF, 깜빡임 OFF
#define LCD_FUNCTION_SET 0x38    // 8비트 데이터, 2라인, 5x8 폰트 설정
#define LCD_SET_DDRAM_ADDR 0x80  // DDRAM 주소 설정 명령어 (커서 위치 설정용)

//------------------------------------------------------------------------------
// LCD 명령어를 LCD에 전송하는 함수
// 파라미터: cmd - LCD에 보낼 명령어 코드
// RS=0 (명령어), RW=0 (쓰기) 상태로 데이터 포트에 명령어 출력 후
// EN핀에 펄스를 줘서 명령어가 인식되도록 함
//------------------------------------------------------------------------------
void lcd_command(uint8_t cmd) {
    LCD_DATA_PORT = cmd;             // PORTB에 명령어 출력 (D0~D7)
    
    LCD_CTRL_PORT &= ~_BV(LCD_RS);  // RS = 0 (명령어 모드)
    LCD_CTRL_PORT &= ~_BV(LCD_RW);  // RW = 0 (쓰기 모드)
    
    LCD_CTRL_PORT |= _BV(LCD_EN);   // EN = 1 (Enable 신호 시작)
    _delay_us(1);                   // 최소 450ns 이상 대기 (Enable 최소 High 시간)
    LCD_CTRL_PORT &= ~_BV(LCD_EN);  // EN = 0 (Enable 신호 종료, LCD가 명령어 인식)
    
    _delay_ms(2);                   // 명령어 처리 시간 대기 (최소 1.5ms 권장)
}

//------------------------------------------------------------------------------
// LCD 데이터(문자)를 LCD에 전송하는 함수
// 파라미터: data - LCD에 출력할 문자(ASCII 코드)
// RS=1 (데이터 모드), RW=0 (쓰기) 상태로 데이터 포트에 문자 출력 후
// EN핀에 펄스를 줘서 데이터가 인식되도록 함
//------------------------------------------------------------------------------
void lcd_data(uint8_t data) {
    LCD_DATA_PORT = data;            // PORTB에 문자 데이터 출력
    
    LCD_CTRL_PORT |= _BV(LCD_RS);   // RS = 1 (데이터 모드)
    LCD_CTRL_PORT &= ~_BV(LCD_RW);  // RW = 0 (쓰기 모드)
    
    LCD_CTRL_PORT |= _BV(LCD_EN);   // EN = 1 (Enable 신호 시작)
    _delay_us(1);                   // 최소 450ns 이상 대기
    LCD_CTRL_PORT &= ~_BV(LCD_EN);  // EN = 0 (Enable 신호 종료, LCD가 데이터 인식)
    
    _delay_ms(2);                   // 데이터 처리 시간 대기
}

//------------------------------------------------------------------------------
// LCD 초기화 함수
// LCD와 MCU 포트 방향 설정, LCD 초기 명령어 전송 및 설정
//------------------------------------------------------------------------------
void lcd_init(void) {
    LCD_DATA_DDR = 0xFF;                        // PORTB 전체 8비트 출력으로 설정 (데이터핀)
    LCD_CTRL_DDR |= _BV(LCD_RS) | _BV(LCD_RW) | _BV(LCD_EN); // PG0,1,2 출력으로 설정 (제어핀)
    
    _delay_ms(20);     // LCD 전원 안정화 대기 (최소 15~20ms 권장)
    
    lcd_command(LCD_FUNCTION_SET);  // LCD 기능 설정 (8비트, 2라인, 5x8 폰트)
    lcd_command(LCD_DISPLAY_ON);    // 디스플레이 ON, 커서 OFF, 깜빡임 OFF
    lcd_command(LCD_CLEAR);         // 화면 클리어 (커서 0,0 위치)
    lcd_command(LCD_ENTRY_MODE);    // 문자 입력 시 커서 오른쪽 이동
}

//------------------------------------------------------------------------------
// LCD 커서를 특정 위치로 이동하는 함수
// row: 0 또는 1 (1행 또는 2행)
// col: 0~15 (열 위치)
//------------------------------------------------------------------------------
void lcd_goto(uint8_t row, uint8_t col) {
    uint8_t addr;
    
    // LCD DDRAM 주소는 1행 시작이 0x00, 2행 시작이 0x40임
    if(row == 0)
        addr = 0x00 + col;  // 1행 위치 계산
    else
        addr = 0x40 + col;  // 2행 위치 계산
    
    lcd_command(LCD_SET_DDRAM_ADDR | addr); // 커서 위치 설정 명령어 전송
}

//------------------------------------------------------------------------------
// 문자열 출력 함수
// 입력받은 문자열 끝까지 한 글자씩 lcd_data 함수로 출력
//------------------------------------------------------------------------------
void lcd_string(const char* str) {
    while(*str) {
        lcd_data(*str++);  // 현재 문자를 LCD에 출력하고 다음 문자로 이동
    }
}

//------------------------------------------------------------------------------
// 메인 함수
//------------------------------------------------------------------------------
int main(void) {
    lcd_init();         // LCD 초기화 수행
    
    lcd_goto(0, 0);     // 1행 1열 위치로 커서 이동
    lcd_string("Hello"); // "Hello" 문자열 LCD에 출력
    
    lcd_goto(1, 0);     // 2행 1열 위치로 커서 이동
    lcd_string("LCD Test2"); // "LCD Test" 문자열 출력
    
    while(1) {
        // LCD 출력 유지 위한 무한루프
    }
    
    return 0;
}
