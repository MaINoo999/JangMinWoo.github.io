/*
 * ADC.c
 *
 * Created: 2025-08-20 오후 1:37:47
 * Author : COMPUTER
 */ 


#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// 7-Segment 표시용 숫자 폰트 (공통 애노드 기준)
unsigned char Font[18] = {
	0x3F, 0x06, 0x5B, 0x4F,
	0x66, 0x6D, 0x7C, 0x07,
	0x7F, 0x67, 0x77, 0x7C,
	0x39, 0x5E, 0x79, 0x71,
	0x08, 0x80
};

unsigned int adc_data = 0;

// 7-Segment 표시 함수
void Segment(int N) {
	int i;
	unsigned char N1000, N100, N10, N1;
	int Buff;

	N1000 = N / 1000;
	Buff = N % 1000;

	N100 = Buff / 100;
	Buff = Buff % 100;

	N10 = Buff / 10;
	N1 = Buff % 10;

	for (i = 0; i < 30; i++) {
		PORTC = 0x0E;	// 첫째 자리
		PORTA = Font[N1000];
		_delay_ms(1);

		PORTC = 0x0D;	// 둘째 자리
		PORTA = Font[N100];
		_delay_ms(1);

		PORTC = 0x0B;	// 셋째 자리
		PORTA = Font[N10];
		_delay_ms(1);

		PORTC = 0x07;	// 넷째 자리
		PORTA = Font[N1];
		_delay_ms(1);
	}
}

// ADC 변환 완료 인터럽트 서비스 루틴
ISR(ADC_vect) {
	adc_data = ADCW;			 // ADC 결과 저장
	ADCSRA |= (1 << ADSC);		 // 다음 변환 시작
}

int main(void) {
	// 7-Segment 출력 포트 설정
	DDRA = 0xFF;	// PORTA = 세그먼트 출력
	PORTA = 0x00;

	DDRC = 0x0F;	// PORTC 하위 4비트 = 자리 선택 출력
	PORTC = 0x0F;

	// ADC 포트 설정 (PF0 = ADC0 입력)
	DDRF &= ~(1 << PF0);	// 입력 설정
	PORTF &= ~(1 << PF0);	// 풀업 끔

	// ADC 설정
	ADMUX = 0x00;						 // AVCC 기준전압, ADC0 채널
	ADCSRA = (1 << ADEN)  |				 // ADC 활성화
	(1 << ADIE)  |				 // 변환 완료 인터럽트 허용
	(1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 분주비 128

	sei();								 // 전역 인터럽트 허용
	ADCSRA |= (1 << ADSC);				 // 첫 번째 변환 시작

	while (1) {
		Segment(adc_data);				 // ADC 값 표시
	}
}
