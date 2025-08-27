/*
 * Timer2.c
 * Created: 2025-08-18 오전
 * Author : COMPUTER
 */

#define F_CPU 16000000UL  // 시스템 클럭 16MHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// 7세그먼트 폰트 배열 (공통 애노드 기준)
unsigned char Font[18] = {
	0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D,
	0x7C, 0x07, 0x7F, 0x67, 0x77, 0x7C,
	0x39, 0x5E, 0x79, 0x71, 0x08, 0x80
};

// 전역 카운터 변수
volatile int t_cnt = 0;
volatile int m_cnt = 0;

// 숫자 표시 함수 (4자리 세그먼트)
void Segment(int num) {
	unsigned char digits[4];
	int i;

	// 숫자 분해
	digits[0] = num / 1000;
	num %= 1000;
	digits[1] = num / 100;
	num %= 100;
	digits[2] = num / 10;
	digits[3] = num % 10;

	// 4자리 출력 (스캔 방식)
	for (i = 0; i < 50; i++) {
		PORTC = 0x0E;
		PORTA = Font[digits[0]];
		_delay_ms(1);

		PORTC = 0x0D;
		PORTA = Font[digits[1]];
		_delay_ms(1);

		PORTC = 0x0B;
		PORTA = Font[digits[2]];
		_delay_ms(1);

		PORTC = 0x07;
		PORTA = Font[digits[3]];
		_delay_ms(1);
	}
}

// 타이머0 오버플로우 인터럽트
ISR(TIMER0_OVF_vect) {
	TCNT0 = 0xC2;  // 초기값 설정 (256 - 62 = 194 → 0xC2)

	t_cnt++;
	if (t_cnt >= 1000) {
		t_cnt = 0;
		m_cnt++;  // 1초마다 증가
	}
}

int main(void) {
	// 포트 설정
	DDRA = 0xFF;  // 7세그먼트 데이터
	DDRC = 0x0F;  // 디지트 선택
	PORTA = 0xFF;
	PORTC = 0x0F;

	// 타이머 설정 (분주비 256)
	ASSR = 0x00;       // 비동기 타이머 사용 안 함
	TCCR0 = 0x04;      // 분주비 256 (Normal mode)
	TIMSK = 0x01;      // Timer0 OVF 인터럽트 허용
	TCNT0 = 0xC2;      // 1ms 주기용 초기값
	TIFR = 0x00;       // 인터럽트 플래그 클리어

	sei();             // 전역 인터럽트 활성화

	while (1) {
		Segment(m_cnt);  // m_cnt 값을 세그먼트에 출력
	}
}
