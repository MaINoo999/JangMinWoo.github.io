#define F_CPU 14745600UL  // MCU 클럭 주파수 14.7456MHz
#include <avr/io.h>
#include <util/delay.h>
#include "lcd/lcd.h"  // LCD 헤더 파일

#define ROWS 4 // 행 (입력)
#define COLS 3 // 열 (출력)

void Keypad_Init() {
	DDRE = 0xF0;  // 상위 4비트 출력, 하위 4비트 입력
}

char keypad(){
	// 첫 번째 행 활성화
	PORTE = 0x10;
	_delay_us(20);
	if (!(PINE & 0x10)) return '1';
	if (!(PINE & 0x20)) return '4';
	if (!(PINE & 0x40)) return '7';
	if (!(PINE & 0x80)) return '*';

	// 두 번째 행 활성화
	PORTE = 0x20;
	_delay_us(20);
	if (!(PINE & 0x10)) return '2';
	if (!(PINE & 0x20)) return '5';
	if (!(PINE & 0x40)) return '8';
	if (!(PINE & 0x80)) return '0';

	// 세 번째 행 활성화
	PORTE = 0x40;
	_delay_us(20);
	if (!(PINE & 0x10)) return '3';
	if (!(PINE & 0x20)) return '6';
	if (!(PINE & 0x40)) return '9';
	if (!(PINE & 0x80)) return '#';
	return 0;
}

int main(void)
{
	char str[] = "Keypad Test";
	Port_Init();
	LCD_Init();     // LCD 초기화
	Keypad_Init();  // 키패드 초기화
	LCD_pos(0,0);
	LCD_STR(str);   // "Keypad Test" 문자열 출력

	while (1) {
		char ch = keypad();
		if(ch) {
			LCD_pos(1, 0); // 출력 위치 초기화
			LCD_CHAR(ch);  // 키 입력 값 출력
		}
	}
}
