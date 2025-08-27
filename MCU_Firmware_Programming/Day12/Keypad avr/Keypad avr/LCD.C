/*
 * IncFile1.h
 *
 * Created: 2025-08-21 오후 1:26:25
 *  Author: COMPUTER
 *
 * 목적: LCD 제어를 위한 초기화 및 출력 함수들 정의
 */

#define F_CPU 14745600UL          // 시스템 클럭 주파수 정의 (딜레이 계산에 사용)

#include <avr/io.h>               // AVR 입출력 헤더
#include <util/delay.h>           // 딜레이 함수 사용을 위한 헤더

// === 포트 초기화 함수 ===
void Port_Init(void) {
	DDRB = 0xFF;     // PORTC를 출력으로 설정 (LCD 데이터용)
	DDRG = 0x0F;     // PORTG의 하위 4비트를 출력으로 설정 (LCD 제어용)
}

// === LCD에 데이터 쓰기 함수 ===
void LCD_Data(Byte ch) {
	LCD_CTRL |= (1 << LCD_RS);     // RS = 1 (데이터 모드)
	LCD_CTRL &= ~(1 << LCD_RW);    // RW = 0 (쓰기 모드)
	LCD_CTRL |= (1 << LCD_EN);     // EN = 1 (LCD 동작 트리거)
	_delay_us(50);                 // 타이밍 딜레이
	LCD_WDATA = ch;                // 데이터 전송
	_delay_us(50);
	LCD_CTRL &= ~(1 << LCD_EN);    // EN = 0 (전송 종료)
}

// === LCD에 명령어 쓰기 함수 ===
void LCD_Comm(Byte ch) {
	LCD_CTRL &= ~(1 << LCD_RS);    // RS = 0 (명령어 모드)
	LCD_CTRL &= ~(1 << LCD_RW);    // RW = 0 (쓰기 모드)
	LCD_CTRL |= (1 << LCD_EN);     // EN = 1 (LCD 동작 트리거)
	_delay_us(50);
	LCD_WINST = ch;                // 명령어 전송
	_delay_us(50);
	LCD_CTRL &= ~(1 << LCD_EN);    // EN = 0 (전송 종료)
}

// === 문자 1개 출력 ===
void LCD_CHAR(Byte c) {
	// 문자 하나를 LCD에 출력
	LCD_Data(c);
	_delay_ms(1);
}

// === 문자열 출력 함수 ===
void LCD_STR(Byte *str) {
	// 문자열의 끝(null)이 나올 때까지 문자 출력 반복
	while (*str != 0) {
		LCD_CHAR(*str);
		str++;
	}
}

// === LCD 커서 위치 설정 ===
void LCD_pos(unsigned char col, unsigned char row) {
	// col = 열 (0~15), row = 행 (0 또는 1)
	// LCD의 DDRAM 주소를 설정하여 커서 위치 이동
	LCD_Comm(0x80 | (row + col * 0x40));  // 행,열에 따라 주소 계산
}

// === LCD 화면 지우기 ===
void LCD_Clear(void) {
	LCD_Comm(0x01);      // Clear Display 명령
	_delay_ms(2);        // 명령 처리 시간 (1.6ms 이상 필요)
}

// === LCD 초기화 함수 ===
void LCD_Init(void) {
	// LCD 동작 모드 설정 (8비트 모드, 2라인, 5x7 도트 폰트)
	LCD_Comm(0x38);
	_delay_ms(2);

	LCD_Comm(0x38);      // 동일 명령 반복 (신뢰성 확보)
	_delay_ms(2);

	LCD_Comm(0x38);
	_delay_ms(2);

	LCD_Comm(0x0E);      // 디스플레이 ON, 커서 ON, 깜빡임 OFF
	_delay_ms(2);

	LCD_Comm(0x06);      // 문자 출력 시 커서 오른쪽 이동
	_delay_ms(2);

	LCD_Clear();         // LCD 화면 초기화
}
