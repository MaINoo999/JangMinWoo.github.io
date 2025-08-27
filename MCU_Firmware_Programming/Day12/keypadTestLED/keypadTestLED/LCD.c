#define F_CPU 14745600UL
#include "LCD.h"              // LCD 헤더 포함

// --- 포트 초기화 함수 ---
void Port_Init(void) {
	// LCD 제어 핀 PG0, PG1, PG2 출력 설정 (RS, RW, EN)
	LCD_CTRL_DDR |= (1 << LCD_RS) | (1 << LCD_RW) | (1 << LCD_EN);

	// LCD 데이터 핀 PG3~PG6 출력 설정
	LCD_DATA_DDR |= LCD_DATA_MASK;

	// 초기 제어 핀과 데이터 핀 모두 LOW로 설정
	LCD_CTRL &= ~((1 << LCD_RS) | (1 << LCD_RW) | (1 << LCD_EN));
	LCD_DATA_PORT &= ~LCD_DATA_MASK;
}

// 4비트 데이터 출력 함수 (하위 4비트 사용)
void LCD_Send4Bits(Byte data) {
	// PG3~PG6 비트 클리어
	LCD_DATA_PORT &= ~LCD_DATA_MASK;

	// data의 각 비트를 PG3~PG6에 매핑해서 출력
	if (data & 0x01) LCD_DATA_PORT |= (1 << PG3);
	if (data & 0x02) LCD_DATA_PORT |= (1 << PG4);
	if (data & 0x04) LCD_DATA_PORT |= (1 << PG5);
	if (data & 0x08) LCD_DATA_PORT |= (1 << PG6);
}

// EN 펄스 발생 함수 (LCD에 데이터/명령어 전송 신호)
void LCD_EnablePulse(void) {
	LCD_CTRL |= (1 << LCD_EN);   // EN HIGH
	_delay_us(1);                // 짧은 딜레이
	LCD_CTRL &= ~(1 << LCD_EN);  // EN LOW
	_delay_us(50);               // 처리 대기
}

// 명령어 전송 함수
void LCD_Comm(Byte cmd) {
	LCD_CTRL &= ~(1 << LCD_RS);   // RS=0: 명령어 모드
	LCD_CTRL &= ~(1 << LCD_RW);   // RW=0: 쓰기 모드

	LCD_Send4Bits(cmd >> 4);      // 상위 4비트 전송
	LCD_EnablePulse();

	LCD_Send4Bits(cmd & 0x0F);    // 하위 4비트 전송
	LCD_EnablePulse();

	_delay_ms(2);                 // 명령어 처리 시간 대기
}

// 데이터 전송 함수
void LCD_Data(Byte data) {
	LCD_CTRL |= (1 << LCD_RS);    // RS=1: 데이터 모드
	LCD_CTRL &= ~(1 << LCD_RW);   // RW=0: 쓰기 모드

	LCD_Send4Bits(data >> 4);     // 상위 4비트 전송
	LCD_EnablePulse();

	LCD_Send4Bits(data & 0x0F);   // 하위 4비트 전송
	LCD_EnablePulse();

	_delay_ms(2);                 // 데이터 처리 시간 대기
}

// 문자 출력 함수 (한 문자 출력)
void LCD_CHAR(Byte c) {
	LCD_Data(c);
	_delay_ms(1);
}

// 문자열 출력 함수
void LCD_STR(const char* str) {
	while (*str) {
		LCD_CHAR(*str++);
	}
}

// 커서 위치 지정 함수
void LCD_pos(Byte col, Byte row) {
	Byte address;
	if (row == 0)
	address = 0x00 + col;   // 1행 주소
	else
	address = 0x40 + col;   // 2행 주소
	LCD_Comm(0x80 | address);   // DDRAM 주소 설정 명령어 전송
}

// LCD 클리어 함수
void LCD_Clear(void) {
	LCD_Comm(0x01);  // 클리어 명령어
	_delay_ms(2);
}

// LCD 초기화 함수
void LCD_Init(void) {
	Port_Init();     // 포트 초기화

	_delay_ms(20);   // 전원 안정화 대기

	// LCD 4비트 초기화 시퀀스 (3번 0x03 보내기)
	LCD_Send4Bits(0x03);
	LCD_EnablePulse();
	_delay_ms(5);

	LCD_EnablePulse();
	_delay_ms(5);

	LCD_EnablePulse();
	_delay_ms(5);

	LCD_Send4Bits(0x02);  // 4비트 모드 설정
	LCD_EnablePulse();
	_delay_ms(5);

	// 기능 설정: 4비트, 2라인, 5x7 폰트
	LCD_Comm(0x28);
	// 디스플레이 ON, 커서 OFF, 깜빡임 OFF
	LCD_Comm(0x0C);
	// 입력 모드: 커서 자동 증가
	LCD_Comm(0x06);
	LCD_Clear();  // 화면 클리어
}
