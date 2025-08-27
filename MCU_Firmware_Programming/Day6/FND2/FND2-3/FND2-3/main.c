#include <avr/io.h>
#include <util/delay.h>

// 7세그먼트 폰트 배열 (공통 캐소드용)
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

// 숫자를 7세그에 출력하는 함수
void display_number(uint16_t number) {
	unsigned char digits[4];

	digits[0] = number % 10;
	digits[1] = (number / 10) % 10;
	digits[2] = (number / 100) % 10;
	digits[3] = (number / 1000) % 10;

	for (int i = 0; i < 4; i++) {
		PORTB = Font[digits[i]];  // 세그먼트 출력
		PORTG = digitSelect[i];   // 자릿수 선택 (LOW)
		_delay_ms(2);
	}

	PORTG = 0xFF; // 자릿수 OFF
}

// FND OFF 함수
void fnd_off() {
	PORTB = 0x00;  // 세그먼트 OFF
	PORTG = 0xFF;  // 모든 자릿수 OFF
}

int main(void)
{
	// 포트 설정
	DDRB = 0xFF;   // PORTB: FND 세그먼트 출력
	DDRG = 0xFF;   // PORTG: FND 자릿수 제어
	DDRE = 0xFF;   // PORTE: LED 출력
	DDRD = 0x00;   // PORTD: 버튼 입력
	PORTD = 0xFF;  // 내부 풀업 저항 활성화

	PORTE = 0x00;  // LED OFF 초기화
	PORTG = 0xFF;  // 모든 자릿수 OFF
	PORTB = 0x00;  // 세그먼트 OFF

	uint16_t counter = 0;
	uint8_t prev_buttons = 0xFF;

	while (1)
	{
		uint8_t buttons = PIND;

		// 버튼 눌림 감지 (이전엔 HIGH, 지금 LOW → 새롭게 눌린 상태)
		uint8_t pressed = (prev_buttons & ~buttons);

		if (pressed != 0) {
			if (pressed & (1 << 0)) {  // 8번 버튼 눌림
				counter = 0;           // 숫자 초기화
				} else if (counter < 9999) {
				counter++;             // 나머지 버튼은 카운터 증가
			}
		}

		prev_buttons = buttons;

		// LED 상태 업데이트 (버튼과 1:1 매칭)
		PORTE = buttons;

		// 버튼이 눌려 있으면 FND 표시
		if (buttons != 0xFF) {
			display_number(counter);
			} else {
			fnd_off();  // 버튼 안 눌렸으면 FND OFF
		}
	}
}
