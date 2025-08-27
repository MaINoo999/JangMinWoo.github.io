#include "keypad.h"

// keypad_map 배열을 이 파일에서 정의합니다.
const char keypad_map[4][3] = {
	{'1', '2', '3'},  // 0번째 행: '1', '2', '3'
	{'4', '5', '6'},  // 1번째 행: '4', '5', '6'
	{'7', '8', '9'},  // 2번째 행: '7', '8', '9'
	{'*', '0', '#'}   // 3번째 행: '*', '0', '#'
};

// Keypad_Init 함수: 키패드를 초기화하는 함수
void Keypad_Init() {
	KEYPAD_DDR = 0xF0;         // 상위 4비트 (컬럼) 출력, 하위 4비트 (행) 입력
	// 처음에 모든 컬럼을 'Low'로 설정하여 초기 상태를 만듬
	KEYPAD_PORT &= ~((1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7));
}

// keypad_get_char 함수: 키패드에서 눌린 버튼을 확인하는 함수
char keypad_get_char(void) {
	char key = '\0'; // 초기값은 '\0' (아무것도 눌리지 않았다는 의미)

	// 컬럼을 차례로 'High'로 만들어서 각 행을 확인
	unsigned char col_select_pattern[] = {
		0x10, // PD4를 High로, 나머지는 Low
		0x20, // PD5를 High로, 나머지는 Low
		0x40  // PD6를 High로, 나머지는 Low
	};

	// 각 컬럼을 순서대로 확인
	for (int c = 0; c < 3; c++) {
		// 컬럼을 선택하고, 다른 컬럼은 Low로 설정
		KEYPAD_PORT = (KEYPAD_PORT & 0x0F) | col_select_pattern[c];
		_delay_us(5); // 안정화 대기 시간

		// 각 행에서 눌린 키 확인
		if ((KEYPAD_PIN & ROW0_PIN_MASK) == ROW0_PIN_MASK) { key = keypad_map[0][c]; break; }
		if ((KEYPAD_PIN & ROW1_PIN_MASK) == ROW1_PIN_MASK) { key = keypad_map[1][c]; break; }
		if ((KEYPAD_PIN & ROW2_PIN_MASK) == ROW2_PIN_MASK) { key = keypad_map[2][c]; break; }
		if ((KEYPAD_PIN & ROW3_PIN_MASK) == ROW3_PIN_MASK) { key = keypad_map[3][c]; break; }
	}

	// 스캔 후 컬럼을 다시 모두 'Low'로 설정하여 초기 상태로 복귀
	KEYPAD_PORT &= ~((1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7));

	return key; // 눌린 키를 반환
}
