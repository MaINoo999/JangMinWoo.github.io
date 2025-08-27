#include "lcd.h"

// 포트 초기화 함수
void Port_Init(void) {
	DDRC = 0xFF;	// PORTC를 출력으로 설정 (LCD 데이터 핀)
	DDRG = 0x0F;	// PORTG의 하위 4비트를 출력으로 설정 (PG0, PG1, PG2는 LCD 제어핀)
}

// LCD에 데이터를 전송하는 함수
void LCD_Data(Byte ch) {
	LCD_CTRL |= (1 << LCD_RS);  // RS=1 (데이터 모드)
	LCD_CTRL &= ~(1 << LCD_RW); // RW=0 (쓰기 모드)
	LCD_CTRL |= (1 << LCD_EN);  // EN High (데이터 전송 시작)
	_delay_us(50); // 딜레이 (이 딜레이는 데이터 전송 완료 후 잠시 기다리는 시간)
	LCD_WDATA = ch;             // 데이터 출력
	_delay_us(50); // 다시 딜레이
	LCD_CTRL &= ~(1 << LCD_EN); // EN Low (데이터 전송 완료)
}

// LCD에 명령어를 전송하는 함수
void LCD_Comm(Byte ch) {
	LCD_CTRL &= ~(1 << LCD_RS); // RS=0 (명령 모드)
	LCD_CTRL &= ~(1 << LCD_RW); // RW=0 (쓰기 모드)
	LCD_CTRL |= (1 << LCD_EN);  // EN High (명령 전송 시작)
	_delay_us(50); // 딜레이
	LCD_WINST = ch;             // 명령어 출력
	_delay_us(50); // 딜레이
	LCD_CTRL &= ~(1 << LCD_EN); // EN Low (명령 전송 완료)
}

// LCD에 문자 하나를 출력하는 함수
void LCD_CHAR(Byte c) {
	// CGROM 문자코드의 0x31 ~ 0xFF는 아스키코드와 일치함
	LCD_Data(c);  // 문자 데이터를 LCD로 출력
	_delay_ms(1); // 출력 후 딜레이
}

// LCD에 문자열을 출력하는 함수
void LCD_STR(Byte *str) {
	// 문자열을 한 문자씩 출력 함수로 전달
	while (*str != 0) {
		LCD_CHAR(*str); // 문자를 하나씩 LCD로 출력
		str++; // 다음 문자로 이동
	}
}

// LCD에서 특정 위치로 커서를 이동시키는 함수 (열, 행 순서)
void LCD_pos(unsigned char col, unsigned char row) {
	unsigned char address;
	if (row == 0) {
		address = 0x80 + col; // 첫 번째 줄 (DDRAM 주소 0x00 - 0x0F에 해당)
		} else { // row == 1
		address = 0xC0 + col; // 두 번째 줄 (DDRAM 주소 0x40 - 0x4F에 해당)
	}
	LCD_Comm(address); // 계산된 주소로 커서 이동
}

// LCD 화면을 클리어하는 함수
void LCD_Clear(void) {
	// 화면을 클리어하는 명령어 0x01
	LCD_Comm(0x01);
	_delay_ms(2); // 1.6ms 이상의 실행시간으로 딜레이 필요
}

// LCD 초기화 함수
void LCD_Init(void) {
	// LCD 초기화 과정 (이미 Port_Init에서 설정되었으므로 생략 가능)
	// LCD_Comm(0x38); // 데이터 8비트 사용, 5X7도트, LCD 2열로 사용 (Function set)
	_delay_ms(2);
	LCD_Comm(0x38); // 함수 설정 (Function Set)
	_delay_ms(2);
	LCD_Comm(0x38); // 함수 설정 (Function Set) 재설정
	_delay_ms(2);
	LCD_Comm(0x0e); // Display ON, Cursor ON, Blink OFF (Display on/off control)
	_delay_ms(2);
	LCD_Comm(0x06); // Increment cursor, No display shift (Entry mode set)
	_delay_ms(2);
	LCD_Clear(); // LCD 화면 초기화
}
