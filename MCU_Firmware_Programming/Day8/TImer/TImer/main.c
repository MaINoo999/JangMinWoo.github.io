#define F_CPU 14745600UL
#include <avr/io.h>
#include <avr/interrupt.h>

volatile int cnt = 0;
volatile int count = 0;
unsigned char LED_Data = 0x00;

// TIMER0 오버플로우 인터럽트 서비스 루틴
ISR(TIMER0_OVF_vect) {
	// 인터럽트 내에서 cli(), sei() 호출 불필요 (자동 처리됨)

	TCNT0 = 0xF2;  // 타이머 초기값 재설정 (오버플로우까지 14 tick)

	cnt++;
	if (cnt >= 1000) {
		cnt = 0;
		count++;
		LED_Data ^= 0xFF;  // LED 데이터 토글 (예: LED 깜빡임)
	}
}

int main(void) {
	DDRB = 0xFF; // PORTB를 출력으로 설정 (LED 연결 가정)

	// 타이머0 초기화
	TCCR0 = 0x00;     // 먼저 초기화
	TCNT0 = 0xF2;     // 초기값 설정 (오버플로우까지 14 tick)
	TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00); // 분주비 1024 설정

	TIMSK |= (1 << TOIE0);  // 타이머0 오버플로우 인터럽트 활성화
	TIFR = 0;               // 인터럽트 플래그 초기화

	sei();                  // 전역 인터럽트 허용

	while (1) {
		PORTB = LED_Data;   // LED 출력
	}

	return 0;
}
