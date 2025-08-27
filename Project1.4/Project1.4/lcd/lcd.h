#ifndef LCD_H_
#define LCD_H_

// F_CPU를 정의하여 AVR에서 사용하는 클럭 주파수를 설정
#define F_CPU 14745600UL // 클럭 주파수 정의 (14.7456MHz)

// AVR 라이브러리 포함
#include <avr/io.h>  // I/O 포트 정의
#include <util/delay.h> // 지연 함수 (_delay_ms(), _delay_us() 등)

// LCD 핀 정의
#define LCD_WDATA PORTC   // LCD 데이터 포트 (D0-D7)
#define LCD_WINST PORTC   // LCD 명령어 포트 (데이터와 동일한 포트 사용)
#define LCD_CTRL PORTG    // LCD 제어 포트 (RS, RW, EN)

// 제어 핀 인덱스 정의
#define LCD_RS 0   // RS 핀 인덱스 (PG0) -> 데이터 모드/명령 모드 선택
#define LCD_RW 1   // RW 핀 인덱스 (PG1) -> 읽기/쓰기 모드 선택
#define LCD_EN 2   // EN 핀 인덱스 (PG2) -> Enable 신호 (데이터 전송 활성화)

// 바이트 타입 정의 (호환성 있는 unsigned char로 정의)
#define Byte unsigned char

// 함수 선언 (이 함수들은 LCD 제어를 위한 다양한 작업을 수행합니다)
void Port_Init(void);          // LCD 포트 초기화 함수 (DDRC = 0xFF, DDRG = 0x0F 설정)
void LCD_Data(Byte);           // LCD에 데이터 쓰기 함수
void LCD_Comm(Byte);           // LCD에 명령어 쓰기 함수
void LCD_CHAR(Byte);           // LCD에 문자 1개 출력 함수
void LCD_STR(Byte*);           // LCD에 문자열 출력 함수
void LCD_pos(unsigned char col, unsigned char row); // LCD 커서 위치 설정 함수 (col, row 순서)
void LCD_Clear(void);          // LCD 화면 클리어 함수
void LCD_Init(void);           // LCD 초기화 함수

#endif /* LCD_H_ */
