/*
 * lcd.c
 *
 * Created: 2025-08-20 오후 1:16:06
 *  Author: COMPUTER
 */ 

#define F_CPU 14745600UL  // MCU의 클럭 주파수를 14.7456 MHz로 설정
#include <avr/io.h>  // AVR 입출력 라이브러리
#include <util/delay.h>  // 지연 함수 라이브러리
#include "lcd.h"  // LCD에 필요한 함수가 정의된 헤더 파일 포함

// 포트 초기화 함수
void Port_Init(void)
{
	DDRG = 0x0f;  // 포트 G의 하위 4비트를 출력으로 설정 (LCD의 데이터 핀에 연결될 수 있음)
	DDRB = 0xff;  // 포트 B 전체를 출력으로 설정 (LCD의 제어 핀에 연결될 수 있음)
}

// LCD에 데이터를 전송하는 함수
void LCD_DATA(Byte data)
{
	LCD_CTRL |= (1 << LCD_RS);  // RS를 1로 설정하여 데이터를 전달할 준비
	LCD_CTRL &= ~(1 << LCD_RW); // RW를 0으로 설정하여 쓰기 모드로 설정
	LCD_CTRL |= (1 << LCD_EN);  // Enable 핀을 1로 설정하여 LCD와 데이터 전송
	_delay_us(50);  // 데이터 안정화를 위한 지연 시간
	LCD_WDATA = data;  // LCD에 데이터 전송
	_delay_us(50);  // 데이터 안정화를 위한 지연 시간
	LCD_CTRL &= ~(1 << LCD_EN);  // Enable 핀을 0으로 설정하여 데이터 전송 종료
}

// LCD에 명령을 전송하는 함수
void LCD_Comm(Byte data)
{
	LCD_CTRL &= ~(1 << LCD_RS);  // RS를 0으로 설정하여 명령을 전달할 준비
	LCD_CTRL &= ~(1 << LCD_RW);  // RW를 0으로 설정하여 쓰기 모드로 설정
	LCD_CTRL |= (1 << LCD_EN);   // Enable 핀을 1로 설정하여 LCD와 데이터 전송
	_delay_us(50);  // 데이터 안정화를 위한 지연 시간
	LCD_WDATA = data;  // LCD에 명령 전송
	_delay_us(50);  // 데이터 안정화를 위한 지연 시간
	LCD_CTRL &= ~(1 << LCD_EN);  // Enable 핀을 0으로 설정하여 데이터 전송 종료
}

// LCD에 한 문자 출력하는 함수
void LCD_CHAR(Byte c)
{
	LCD_DATA(c);  // LCD에 문자 데이터 전송
	_delay_ms(1);  // 출력 안정화를 위한 지연 시간
}

// LCD에 문자열 출력하는 함수
void LCD_STR(Byte* str)
{
	// 문자열이 끝날 때까지 각 문자를 출력
	while(*str != 0){
		LCD_CHAR(*str);  // 현재 문자를 출력
		str++;  // 다음 문자로 이동
	}
}

// LCD 커서 위치 설정 함수 (열, 행 지정)
void LCD_pos(Byte col, Byte row)
{
	// LCD의 주소 계산하여 커서 위치 설정
	LCD_Comm(0x80 | (row + col * 0x40));  // 첫 번째 행은 0x00부터, 두 번째 행은 0x40부터 시작
}

// LCD 화면 지우는 함수
void LCD_Clear(void)
{
	LCD_Comm(0x01);  // 지우기 명령을 LCD에 전송
	_delay_ms(2);  // 화면 지우기 안정화를 위한 지연 시간
}

// LCD 초기화 함수
void LCD_Init(void)
{
	LCD_Comm(0x38);  // LCD를 8비트 모드로 설정
	_delay_ms(2);    // 안정화를 위한 지연 시간
	LCD_Comm(0x38);  // LCD를 다시 8비트 모드로 설정
	_delay_ms(2);    // 안정화를 위한 지연 시간
	LCD_Comm(0x38);  // LCD를 다시 8비트 모드로 설정
	_delay_ms(2);    // 안정화를 위한 지연 시간
	LCD_Comm(0x0e);  // LCD 화면을 켜고, 커서를 표시
	_delay_ms(2);    // 안정화를 위한 지연 시간
	LCD_Comm(0x06);  // 커서 이동 시 화면을 밀지 않도록 설정
	_delay_ms(2);    // 안정화를 위한 지연 시간
	LCD_Clear();     // 화면 초기화
}
