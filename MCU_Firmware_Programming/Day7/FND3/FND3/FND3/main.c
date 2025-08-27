+#include <avr/io.h>
#include <util/delay.h>

// 7세그먼트 폰트 (공통 캐소드 기준)
unsigned char Font[16] = {
	0b00111111, // 0
	0b00000110, // 1
	0b01011011, // 2
	0b01001111, // 3
	0b01100110, // 4
	0b01101101, // 5
	0b01111101, // 6
	0b00000111, // 7
	0b01111111, // 8
	0b01101111, // 9
	0b01110111, // A
	0b01111100, // b
	0b00111001, // C
	0b01011110, // d
	0b01111001, // E
	0b01110001  // F
};

// 자릿수 선택 (PORTG, LOW가 해당 자리 활성화)
unsigned char digitSelect[4] = {
	0b0111, // 천의 자리
	0b1011, // 백의 자리
	0b1101, // 십의 자리
	0b1110  // 일의 자리
};

// 숫자를 FND에 출력
void display_number(uint16_t number) {
	unsigned char digits[4];

	// 숫자 각 자릿수 분해
	digits[3] = number % 10;           // 1의 자리
	digits[2] = (number / 10) % 10;    // 10의 자리
	digits[1] = (number / 100) % 10;   // 100의 자리
	digits[0] = (number / 1000) % 10;  // 1000의 자리

	// 4자리 FND에 출력
	for (int i = 0; i < 4; i++) {
		PORTB = Font[digits[i]];    // 세그먼트 숫자 표시
		PORTG = digitSelect[i];     // 해당 자릿수 선택 (LOW로 활성화)
		_delay_ms(2);               // 짧은 표시 시간
	}

	PORTG = 0xFF;  // 모든 자릿수 비활성화
}

// FND 끄기
void fnd_off() {
	PORTB = 0x00;  // 모든 세그먼트 OFF
	PORTG = 0xFF;  // 자릿수 OFF
}

int main(void) {
	// 포트 설정
	DDRB = 0xFF;   // PORTB: 세그먼트 출력
	DDRG = 0xFF;   // PORTG: 자릿수 선택
	DDRE = 0xFF;   // PORTE: LED 출력
	DDRD = 0x00;   // PORTD: 버튼 입력
	PORTD = 0xFF;  // 내부 풀업 저항 활성화

	// 초기 상태
	PORTE = 0x00;  // LED OFF
	PORTG = 0xFF;  // 자릿수 OFF
	PORTB = 0x00;  // 세그먼트 OFF

	uint16_t counter = 0;         // 전체 숫자
	uint8_t prev_buttons = 0xFF; // 이전 버튼 상태 (처음엔 전부 HIGH)

	while (1) {
		uint8_t buttons = PIND;                 // 현재 버튼 상태 읽기
		uint8_t pressed = prev_buttons & ~buttons; // 이전엔 HIGH, 현재 LOW → 새로 눌린 버튼만 추출

		// 버튼별 동작
		if (pressed & (1 << 0)) {       // PD0: 1의 자리 증가
			if ((counter % 10) < 9) {
				counter += 1;
			}
		}
		if (pressed & (1 << 1)) {       // PD1: 10의 자리 증가
			if (((counter / 10) % 10) < 9) {
				counter += 10;
			}
		}
		if (pressed & (1 << 2)) {       // PD2: 100의 자리 증가
			if (((counter / 100) % 10) < 9) {
				counter += 100;
			}
		}
		if (pressed & (1 << 3)) {       // PD3: 1000의 자리 증가
			if (((counter / 1000) % 10) < 9) {
				counter += 1000;
			}
		}
		if (pressed & (1 << 4)) {       // PD4: 초기화
			counter = 0;
		}

		// 최대 9999로 제한
		if (counter > 9999) counter = 9999;

		prev_buttons = buttons;      // 이전 버튼 상태 갱신
		PORTE = buttons;             // 현재 버튼 상태를 LED로 표시

		// 버튼이 하나라도 눌려 있으면 숫자 표시
		if (buttons != 0xFF) {
			display_number(counter);
			} else {
			fnd_off();  // 버튼 안 눌리면 꺼짐
		}
	}
}
