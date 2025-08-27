#ifndef KEYPAD_H_
#define KEYPAD_H_

#define F_CPU 14745600UL // 클럭 주파수 정의

#include <avr/io.h>
#include <util/delay.h>

// 키패드 연결 핀 정의 (PORTD 사용, DDRD = 0xF0에 맞춤)
// Columns (출력, HIGH로 제어): PD4, PD5, PD6
// Rows (입력, HIGH이면 눌린 것): PD0, PD1, PD2, PD3

// Port Definitions
#define KEYPAD_PORT         PORTD // 키패드 컬럼 출력용 포트 (COLUMNS)
#define KEYPAD_PIN          PIND  // 키패드 행 입력값을 읽기 위한 포트 (ROWS)
#define KEYPAD_DDR          DDRD  // 키패드 핀 입출력 방향 설정

// Column Output Pin Masks (각각의 컬럼을 선택하기 위한 비트마스크)
#define COL0_PIN_MASK        (1 << PD4) // 0x10 (PD4)
#define COL1_PIN_MASK        (1 << PD5) // 0x20 (PD5)
#define COL2_PIN_MASK        (1 << PD6) // 0x40 (PD6)

// Row Input Pin Masks (각각의 행을 선택하기 위한 비트마스크)
#define ROW0_PIN_MASK        (1 << PD0) // 0x01 (PD0)
#define ROW1_PIN_MASK        (1 << PD1) // 0x02 (PD1)
#define ROW2_PIN_MASK        (1 << PD2) // 0x04 (PD2)
#define ROW3_PIN_MASK        (1 << PD3) // 0x08 (PD3)

// 키패드 매핑 (4x3 키패드 버튼에 대한 문자 배열)
extern const char keypad_map[4][3];

void Keypad_Init(void); // 키패드 초기화 함수
char keypad_get_char(void); // 키패드에서 눌린 버튼을 반환하는 함수

#endif /* KEYPAD_H_ */
