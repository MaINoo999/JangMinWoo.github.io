/*
 * ATmega128 + HD44780 LCD 16x2 제어 예제 - 구구단 2단 출력
 * 
 * LCD 핀 연결:
 *  - D0~D7  : PORTB (PB0~PB7)
 *  - RS, RW, EN : PORTG (PG0, PG1, PG2)
 *  - VSS → GND, VDD → 5V, V0 → 가변저항 (10kΩ, 콘트라스트 조절)
 *  - A(LED+) → 5V, K(LED-) → GND (백라이트)
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>  // sprintf 사용을 위해 포함

// LCD 제어 핀 정의
#define LCD_RS PG0
#define LCD_RW PG1
#define LCD_EN PG2

#define LCD_CTRL_DDR DDRG
#define LCD_CTRL_PORT PORTG

// LCD 데이터 핀 정의
#define LCD_DATA_DDR DDRB
#define LCD_DATA_PORT PORTB

// LCD 명령어 정의
#define LCD_CLEAR 0x01
#define LCD_RETURN_HOME 0x02
#define LCD_ENTRY_MODE 0x06
#define LCD_DISPLAY_ON 0x0C
#define LCD_FUNCTION_SET 0x38
#define LCD_SET_DDRAM_ADDR 0x80

// LCD 명령어 전송 함수
void lcd_command(uint8_t cmd) {
    LCD_DATA_PORT = cmd;
    LCD_CTRL_PORT &= ~_BV(LCD_RS); // 명령어 모드
    LCD_CTRL_PORT &= ~_BV(LCD_RW); // 쓰기 모드
    LCD_CTRL_PORT |= _BV(LCD_EN);  // Enable High
    _delay_us(1);
    LCD_CTRL_PORT &= ~_BV(LCD_EN); // Enable Low
    _delay_ms(2); // 처리 시간 대기
}

// LCD 문자 전송 함수
void lcd_data(uint8_t data) {
    LCD_DATA_PORT = data;
    LCD_CTRL_PORT |= _BV(LCD_RS);  // 데이터 모드
    LCD_CTRL_PORT &= ~_BV(LCD_RW); // 쓰기 모드
    LCD_CTRL_PORT |= _BV(LCD_EN);  // Enable High
    _delay_us(1);
    LCD_CTRL_PORT &= ~_BV(LCD_EN); // Enable Low
    _delay_ms(2);
}

// LCD 초기화 함수
void lcd_init(void) {
    LCD_DATA_DDR = 0xFF; // 데이터 포트 전부 출력 설정
    LCD_CTRL_DDR |= _BV(LCD_RS) | _BV(LCD_RW) | _BV(LCD_EN); // 제어핀 출력 설정

    _delay_ms(20);  // LCD 안정화 대기

    lcd_command(LCD_FUNCTION_SET); // 기능 설정: 8비트, 2라인, 5x8 폰트
    lcd_command(LCD_DISPLAY_ON);   // 디스플레이 ON
    lcd_command(LCD_CLEAR);        // 화면 클리어
    lcd_command(LCD_ENTRY_MODE);   // 입력 모드 설정
}

// 커서 위치 설정 함수 (row: 0~1, col: 0~15)
void lcd_goto(uint8_t row, uint8_t col) {
    uint8_t addr = (row == 0) ? 0x00 + col : 0x40 + col;
    lcd_command(LCD_SET_DDRAM_ADDR | addr);
}

// 문자열 출력 함수
void lcd_string(const char* str) {
    while (*str) {
        lcd_data(*str++);
    }
}

// 메인 함수
int main(void) {
    char line[17]; // 문자열 버퍼 (LCD 16자 + 널문자)

    lcd_init(); // LCD 초기화

    lcd_goto(0, 0);             // LCD 1행 첫 번째 칸으로 커서 이동
    lcd_string("GuGuDan 2Dan"); // 제목 출력

    while (1) {
        for (int i = 1; i <= 9; i++) {
            lcd_goto(1, 0);            // 2행으로 커서 이동
            lcd_command(LCD_CLEAR);    // 이전 문자열 클리어
            lcd_goto(0, 0);            
            lcd_string("GuGuDan 2Dan"); // 제목 다시 출력
            lcd_goto(1, 0);            // 2행 위치로 이동

            // 예: "2 x 1 = 2" 형식의 문자열 생성
            sprintf(line, "2 x %d = %2d", i, 2 * i);
            lcd_string(line); // LCD에 문자열 출력

            _delay_ms(1000); // 1초 대기 후 다음 단 출력
        }
    }

    return 0;
}
