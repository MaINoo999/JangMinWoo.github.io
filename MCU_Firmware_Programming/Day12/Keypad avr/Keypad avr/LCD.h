/*
 * IncFile2.h
 *
 * Created: 2025-08-21 오후 1:26:50
 *  Author: COMPUTER
 *
 * 목적: LCD 관련 정의와 함수 원형 선언을 포함하는 헤더 파일
 */

#ifndef LCD_H_         // 중복 포함 방지 시작
#define LCD_H_

// === 포트 매핑 ===
#define LCD_WDATA  PORTB   // LCD에 데이터 출력용 포트
#define LCD_WINST  PORTB   // LCD에 명령어 출력용 포트 (PORTC 공유)
#define LCD_CTRL   PORTG   // LCD 제어 신호용 포트 (RS, RW, EN)

// === LCD 제어 핀 번호 정의 (PORTG의 몇 번 핀을 쓰는지 명시) ===
#define LCD_RS  0          // Register Select (명령/데이터 선택)
#define LCD_RW  1          // Read/Write 선택
#define LCD_EN  2          // Enable (동작 트리거)

// === 사용자 정의 상수 및 타입 ===
#define Byte     unsigned char   // Byte를 unsigned char로 정의
#define On       1               // ON 상태 표현
#define Off      0               // OFF 상태 표현

// === 예전 코드 (주석 처리됨) ===
// #define TLCD_EN { TLCD_E = 0; TLCD_E = 1; }   // 예전 방식의 Enable 토글 정의
// #define DATA PORTC                           // 예전 방식의 데이터 포트 매핑

// === 함수 원형 선언부 ===
// 이 함수들은 main.c 또는 다른 .c 파일에서 호출할 수 있도록 선언해둠

void Port_Init(void);         // 128A 보드의 포트 초기화 함수 (LCD 제어 핀 설정)

void LCD_DATA(Byte);          // LCD에 데이터 1바이트 출력
void LCD_Comm(Byte);          // LCD에 명령어 1바이트 출력
void LCD_CHAR(Byte);          // LCD에 문자 1개 출력
void LCD_STR(Byte*);          // LCD에 문자열 출력
void LCD_pos(Byte, Byte);     // LCD의 커서 위치 설정 (열, 행)
void LCD_Clear(void);         // LCD 화면 지우기
void LCD_Init(void);          // LCD 초기화 함수

#endif /* LCD_H_ */           // 중복 포함 방지 끝
