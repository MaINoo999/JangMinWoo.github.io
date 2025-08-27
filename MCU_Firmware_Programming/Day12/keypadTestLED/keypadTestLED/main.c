#define F_CPU 14745600UL          // CPU 클럭 14.7456 MHz로 설정
#include <avr/io.h>               // AVR 입출력 관련 헤더 포함
#include <util/delay.h>           // _delay_ms 등 딜레이 함수 사용을 위해 포함
#include "LCD.h"                  // LCD 제어 관련 헤더 포함

// --- 키패드 행/열 핀 정의 ---
#define ROW_PORT PIND             // 행 입력 포트: PIND 레지스터 (핀 상태 읽기)
#define ROW_DDR  DDRD             // 행 핀 방향 설정 레지스터
#define ROW_PIN_MASK 0xF0         // 행은 PD4~PD7 (상위 4비트)
#define ROW_SHIFT 4               // 행 핀 위치 오프셋 (4비트 이동)

// 열 핀은 PD0~PD2 사용
#define COL_PORT PORTD            // 열 출력 포트: PORTD (출력 레지스터)
#define COL_DDR  DDRD             // 열 방향 설정 레지스터
#define COL_PIN_MASK 0x07         // 열은 PD0~PD2 (하위 3비트)

// --- 키패드 초기화 함수 ---
void Keypad_Init() {
	// 행 핀은 입력으로 설정하고 풀업 저항 활성화
	ROW_DDR &= ~ROW_PIN_MASK;    // 입력으로 설정 (0)
	PORTD |= ROW_PIN_MASK;       // 풀업 활성화 (1)
	
	// 열 핀은 출력으로 설정하고 초기 출력은 HIGH (비활성 상태)
	COL_DDR |= COL_PIN_MASK;     // 출력 설정
	PORTD |= COL_PIN_MASK;       // HIGH 출력
}

// --- 키패드 스캔 함수 ---
char Keypad_GetKey(void) {
	// 키패드 4행 3열 키맵 정의
	const char keys[4][3] = {
		{'1', '2', '3'},
		{'4', '5', '6'},
		{'7', '8', '9'},
		{'*', '0', '#'}
	};

	for (uint8_t col = 0; col < 3; col++) {
		// 모든 열을 HIGH로 세트
		PORTD |= COL_PIN_MASK;

		// 현재 열만 LOW로 설정하여 해당 열 스캔
		PORTD &= ~(1 << col);

		_delay_us(5);  // 신호 안정화 대기

		for (uint8_t row = 0; row < 4; row++) {
			// 행 핀 중 하나가 LOW라면 (키가 눌린 상태)
			if (!(ROW_PORT & (1 << (row + ROW_SHIFT)))) {
				_delay_ms(20);  // 디바운싱(스위치 떨림 방지) 대기
				if (!(ROW_PORT & (1 << (row + ROW_SHIFT)))) {
					while (!(ROW_PORT & (1 << (row + ROW_SHIFT)))); // 키가 떼질 때까지 대기
					return keys[row][col];  // 눌린 키 문자 반환
				}
			}
		}
	}
	return 0;  // 눌린 키 없으면 0 반환
}

// --- 메인 함수 ---
int main(void) {
	DDRE = 0xFF;      // PE0~PE7 출력 설정 (LED 제어용)
	PORTE = 0x00;     // LED 모두 끄기 (초기화)

	LCD_Init();       // LCD 초기화 함수 호출
	Keypad_Init();    // 키패드 초기화

	LCD_Clear();      // LCD 화면 클리어
	LCD_pos(0, 0);    // LCD 첫 줄 첫 칸으로 커서 이동
	LCD_STR("Enter Password:");  // 안내문 출력

	const char password[] = "1234567";  // 정답 비밀번호
	const uint8_t pass_len = 7;

	char input[8] = {0};   // 입력 받은 키 저장용 배열 (문자열 끝 null 포함)
	uint8_t input_idx = 0; // 입력 위치 인덱스

	while (1) {
		char key = Keypad_GetKey();  // 키패드에서 키값 읽기
		if (key) {                   // 키가 눌렸다면
			_delay_ms(200);          // 디바운싱 및 중복 입력 방지

			if (key != '#') {        // '#'키는 입력 종료 신호
				if (input_idx < pass_len) {
					input[input_idx++] = key;  // 입력 저장
					LCD_pos(input_idx - 1, 1); // 2번째 줄에 입력 자리 이동
					LCD_CHAR('*');             // 실제 키 대신 '*' 출력
				}
			}
			else {
				// 입력 종료 시점: 문자열 끝에 널 문자 추가
				input[input_idx] = '\0';

				// 입력한 비밀번호와 저장된 비밀번호 비교
				uint8_t correct = 1;
				for (uint8_t i = 0; i < pass_len; i++) {
					if (input[i] != password[i]) {
						correct = 0;  // 틀리면 플래그 0
						break;
					}
				}

				LCD_Clear();
				if (correct) {
					LCD_pos(0, 0);
					LCD_STR("Password OK");  // 비밀번호 맞으면 표시
					PORTE = 0xFF;           // LED 모두 켬
				}
				else {
					LCD_pos(0, 0);
					LCD_STR("Password FAIL"); // 틀리면 표시
					PORTE = 0x00;            // LED 모두 끔
				}
				_delay_ms(2000);             // 2초 대기

				// 입력 초기화
				for (uint8_t i = 0; i < 8; i++) input[i] = 0;
				input_idx = 0;

				// 다시 입력 안내문 출력
				LCD_Clear();
				LCD_pos(0, 0);
				LCD_STR("Enter Password:");
			}
		}
	}
}
