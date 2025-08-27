/*
 * Timer2.c
 *
 * Created: 2025-08-18 오전 10:11:29
 * Author : COMPUTER
 */

#define F_CPU 16000000UL  // [1] CPU 클럭 주파수 정의 (16MHz 기준)
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// [2] 7세그먼트 폰트 테이블 (공통 애노드 기준)
unsigned char Font[18] = {
	0x3F, // 0
	0x06, // 1
	0x5B, // 2
	0x4F, // 3
	0x66, // 4
	0x6D, // 5
	0x7C, // 6
	0x07, // 7
	0x7F, // 8
	0x67, // 9
	0x77, // A
	0x7C, // b
	0x39, // C
	0x5E, // d
	0x79, // E
	0x71, // F
	0x08, // -
	0x80  // .
};

// [3] 전역 변수
volatile int t_cnt = 0;  // 1ms 단위 카운터
volatile int m_cnt = 0;  // 1초 단위 카운터

// [4] 숫자 4자리 7세그먼트 출력 함수
void Segment(int num) {
	unsigned char digits[4];
	int i;

	// 숫자 분해 (천, 백, 십, 일)
	digits[0] = num / 1000;
	num %= 1000;
	digits[1] = num / 100;
	num %= 100;
	digits[2] = num / 10;
	digits[3] = num % 10;

	// 각 자릿수에 대해 세그먼트 출력 (스캔 방식)
	for (i = 0; i < 50; i++) {
		// 천의 자리
		PORTC = 0x0E;  // 0000 1110
		PORTA = Font[digits[0]];
		_delay_ms(1);

		// 백의 자리
		PORTC = 0x0D;  // 0000 1101
		PORTA = Font[digits[1]];
		_delay_ms(1);

		// 십의 자리
		PORTC = 0x0B;  // 0000 1011
		PORTA = Font[digits[2]];
		_delay_ms(1);

		// 일의 자리
		PORTC = 0x07;  // 0000 0111
		PORTA = Font[digits[3]];
		_delay_ms(1);
	}
}

// [5] Timer0 오버플로우 인터럽트 (1ms 주기)
ISR(TIMER0_OVF_vect) {
	TCNT0 = 0xF2;  // 타이머 초기값: 1ms 주기 기준

	t_cnt++;
	if (t_cnt >= 1000) {
		t_cnt = 0;
		m_cnt++;  // 1초마다 증가
	}
}

// [6] 메인 함수
int main(void) {
	// 포트 설정
	DDRA = 0xFF;    // 7세그먼트 데이터 출력
	DDRC = 0x0F;    // 4자리 선택 출력
	PORTA = 0xFF;
	PORTC = 0x0F;

	// Timer0 설정 (Normal Mode, 분주비 1024)
	ASSR = 0x00;     // 내부 클럭 사용
	TCCR0 = 0x07;    // 분주비 1024
	TIMSK = 0x01;    // Timer0 OVF 인터럽트 허용
	TCNT0 = 0xF2;    // 1ms 주기
	TIFR = 0x00;     // 인터럽트 플래그 초기화

	sei();           // 전역 인터럽트 활성화

	while (1) {
		Segment(m_cnt);  // 카운트 출력
	}
}
