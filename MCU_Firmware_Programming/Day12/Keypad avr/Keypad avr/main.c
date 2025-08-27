/*
 * Keypad avr.c
 *
 * Created: 2025-08-21 오후 1:15:24
 * Author : COMPUTER
 *
 * 목적: 4x3 키패드에서 입력을 받아 LCD에 해당 문자를 출력하는 프로그램
 */

#define F_CPU 14745600UL          // 시스템 클럭 주파수 (딜레이 계산용)

#include <avr/io.h>               // AVR 입출력 기능
#include <util/delay.h>           // 딜레이 함수 사용
#include "Lcd.h"                  // 사용자 정의 LCD 제어 헤더파일

#define ROWS 4    // 키패드의 행 수 (입력)
#define COLS 3    // 키패드의 열 수 (출력)

// === 키패드 초기화 함수 ===
// 상위 4비트 (PD7~PD4)는 출력으로 설정하여 열(column)을 선택
// 하위 4비트 (PD3~PD0)는 입력으로 설정하고 내부 풀업 저항 활성화
void Keypad_Init() {
	DDRD = 0xF0;     // 11110000: 상위 4비트 출력, 하위 4비트 입력
	PORTD = 0x0F;    // 00001111: 하위 4비트 풀업 저항 활성화
}

// === 현재 LCD 커서 위치 변수 선언 ===
//unsigned char row = 1;       // 출력 시작 행 (1 = 두 번째 줄)
//unsigned char col = 0;       // 출력 시작 열 (0 = 첫 번째 칸)

// === LCD에 문자 출력 및 커서 이동 함수 ===
void print_to_LCD(char c) {
	LCD_pos(row, col);   // 현재 커서 위치로 이동
	LCD_CHAR(c);         // 문자 출력
	col++;               // 커서 열 위치 증가

	// 만약 현재 줄의 끝(16열)을 넘어가면 다음 줄로 이동
	if (col >= 16) {
		col = 0;                   // 열 위치 초기화
		row = (row + 1) % 2;       // 0 또는 1로 줄 변경
		LCD_pos(row, col);         // 커서 이동
	}
}

// === 메인 함수 ===
int main(void)
{
	Byte str[] = "Keypad Test";   // LCD 첫 줄에 출력할 문자열

	Port_Init();     // LCD 관련 포트 초기화 함수 (외부 정의)
	LCD_Init();      // LCD 초기화 함수
	Keypad_Init();   // 키패드 포트 설정

	LCD_pos(0, 0);   // LCD 첫 줄 첫 칸으로 커서 이동
	LCD_STR(str);    // "Keypad Test" 문자열 출력

	LCD_pos(1, 0);   // 두 번째 줄 첫 칸으로 커서 이동하여 입력 시작
	
	while (1)  // 무한 루프
	{
		// === 첫 번째 열 (PD4를 LOW로 설정하여 열 선택) ===
		PORTD = 0x10;   // 00010000: PD4 LOW, 나머지 HIGH
		_delay_us(5);   // 신호 안정화를 위해 짧은 딜레이

		// 각 행 (PD0~PD3)을 확인하여 눌린 키 식별
		if ((PIND & 0x01) == 0x01) {       // PD0가 LOW → '1'
			print_to_LCD('1');
			while ((PIND & 0x01) == 0x01);  // 키에서 손 뗄 때까지 대기
			_delay_ms(100);                 // 디바운싱
		}
		else if ((PIND & 0x02) == 0x02) {  // PD1 → '4'
			print_to_LCD('4');
			while ((PIND & 0x02) == 0x02);
			_delay_ms(100);
		}
		else if ((PIND & 0x04) == 0x04) {  // PD2 → '7'
			print_to_LCD('7');
			while ((PIND & 0x04) == 0x04);
			_delay_ms(100);
		}
		else if ((PIND & 0x08) == 0x08) {  // PD3 → '*'
			print_to_LCD('*');
			while ((PIND & 0x08) == 0x08);
			_delay_ms(100);
		}

		// === 두 번째 열 (PD5 LOW) ===
		PORTD = 0x20;   // 00100000: PD5 LOW
		_delay_us(5);

		if ((PIND & 0x01) == 0x01) {       // PD0 → '2'
			print_to_LCD('2');
			while ((PIND & 0x01) == 0x01);
			_delay_ms(100);
		}
		else if ((PIND & 0x02) == 0x02) {  // PD1 → '5'
			print_to_LCD('5');
			while ((PIND & 0x02) == 0x02);
			_delay_ms(100);
		}
		else if ((PIND & 0x04) == 0x04) {  // PD2 → '8'
			print_to_LCD('8');
			while ((PIND & 0x04) == 0x04);
			_delay_ms(100);
		}
		else if ((PIND & 0x08) == 0x08) {  // PD3 → '0'
			print_to_LCD('0');
			while ((PIND & 0x08) == 0x08);
			_delay_ms(100);
		}

		// === 세 번째 열 (PD6 LOW) ===
		PORTD = 0x40;   // 01000000: PD6 LOW
		_delay_us(5);

		if ((PIND & 0x01) == 0x01) {       // PD0 → '3'
			print_to_LCD('3');
			while ((PIND & 0x01) == 0x01);
			_delay_ms(100);
		}
		else if ((PIND & 0x02) == 0x02) {  // PD1 → '6'
			print_to_LCD('6');
			while ((PIND & 0x02) == 0x02);
			_delay_ms(100);
		}
		else if ((PIND & 0x04) == 0x04) {  // PD2 → '9'
			print_to_LCD('9');
			while ((PIND & 0x04) == 0x04);
			_delay_ms(100);
		}
		else if ((PIND & 0x08) == 0x08) {  // PD3 → '#'
			print_to_LCD('#');
			while ((PIND & 0x08) == 0x08);
			_delay_ms(100);
		}
	}
}
