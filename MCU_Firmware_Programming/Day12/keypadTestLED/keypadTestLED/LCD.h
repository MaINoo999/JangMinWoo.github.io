#ifndef LCD_H_
#define LCD_H_

// PG0~PG7이 정의되어 있지 않으면 직접 정의 (핀 번호)
#ifndef PG0
#define PG0 0
#define PG1 1
#define PG2 2
#define PG3 3
#define PG4 4
#define PG5 5
#define PG6 6
#define PG7 7
#endif

#include <avr/io.h>
#include <util/delay.h>

// LCD 제어 핀 포트 및 핀 번호 정의
#define LCD_CTRL PORTG
#define LCD_CTRL_DDR DDRG

// 데이터 핀 PG3~PG6 (4비트 데이터용)
#define LCD_DATA_PORT PORTG
#define LCD_DATA_DDR DDRG
#define LCD_DATA_MASK ((1 << PG3) | (1 << PG4) | (1 << PG5) | (1 << PG6))

// RS, RW, EN 핀 번호
#define LCD_RS PG0
#define LCD_RW PG1
#define LCD_EN PG2

typedef unsigned char Byte;

// 함수 원형 선언
void Port_Init(void);
void LCD_Comm(Byte cmd);
void LCD_Data(Byte data);
void LCD_CHAR(Byte c);
void LCD_STR(const char* str);
void LCD_pos(Byte col, Byte row);
void LCD_Clear(void);
void LCD_Init(void);

#endif /* LCD_H_ */
