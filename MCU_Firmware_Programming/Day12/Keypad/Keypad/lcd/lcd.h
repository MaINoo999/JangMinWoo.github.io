/*
 * lcd.h
 *
 * Created: 2025-08-20 오후 1:15:47
 *  Author: COMPUTER
 */ 

#ifndef LCD_H_  // 헤더 가드: 이 파일이 여러 번 포함되지 않도록 방지
#define LCD_H

// LCD의 제어 신호 및 데이터 핀을 연결할 포트 정의
#define LCD_WDATA	PORTB      // LCD 데이터 핀 연결 (포트 B)
#define LCD_WINST	PORTB      // (필요시 사용) LCD 데이터 핀을 관리하는 포트
#define LCD_CTRL	PORTG      // LCD 제어 핀 연결 (포트 G)

// LCD 제어 신호 정의
#define LCD_RS  0            // LCD RS 신호 (0: 명령, 1: 데이터)
#define LCD_RW  1            // LCD RW 신호 (0: 쓰기, 1: 읽기)
#define LCD_EN  2           // LCD E 신호 (Enable)

// Byte 타입을 char로 정의 (1바이트 크기)
#define Byte char

// On/Off 상태 정의
#define On	1
#define Off	0

// 함수 프로토타입 (lcd.c에서 구현된 함수들)
void Port_Init(void);        // 포트 초기화 함수 (포트 G와 B 설정)
void LCD_DATA(Byte);         // LCD에 데이터 입력 함수
void LCD_Comm(Byte);         // LCD에 명령 입력 함수
void LCD_CHAR(Byte);         // LCD에 한 문자 출력 함수
void LCD_STR(Byte*);         // LCD에 문자열 출력 함수
void LCD_pos(Byte, Byte);    // LCD 커서 위치 설정 함수
void LCD_Clear(void);        // LCD 화면 지우기 함수
void LCD_Init(void);         // LCD 초기화 함수

#endif /* LCD_H_ */
