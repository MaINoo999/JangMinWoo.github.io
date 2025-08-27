#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

int main(void) {
	DDRA = 0xff;      // PORTA: 출력 (LED)
	PORTA = 0x00;     // 초기 상태: 모든 LED OFF

	DDRD &= ~0xFF;    // PORTD: 입력 (PD0~PD7 버튼)
	PORTD |= 0xFF;    // 내부 풀업 저항 활성화

	uint8_t prev_state = 0xFF;  // 버튼의 이전 상태 (초기엔 모두 HIGH)

	while (1) {
		uint8_t curr_state = PIND;  // 현재 버튼 상태 읽기

		for (uint8_t i = 0; i < 8; i++) {
			uint8_t mask = (1 << i);

			// 이전 상태는 HIGH, 현재는 LOW → 버튼 눌림 발생
			if ((prev_state & mask) && !(curr_state & mask)) {
				PORTA ^= mask;  // 해당 LED 토글
			}
		}

		prev_state = curr_state;   // 이전 상태 갱신
		_delay_ms(10);             // 짧은 디바운스
	}

	return 0;
}
