/*
 * CDS_LED_Display.c
 *
 * Created: 2025-08-20 오후 4:00:00
 * Author : COMPUTER
 * Description: CDS 조도센서 ADC 값을 읽어 7-segment에 표시하고,
 *              밝기에 따라 LED가 단계별로 꺼지는 제어를 수행함
 */

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// 7-Segment 숫자 폰트 테이블 (공통 애노드 기준)
unsigned char Font[18] = {
	0x3F, 0x06, 0x5B, 0x4F,
	0x66, 0x6D, 0x7C, 0x07,
	0x7F, 0x67, 0x77, 0x7C,
	0x39, 0x5E, 0x79, 0x71,
	0x08, 0x80
};

volatile unsigned int adc_data = 0; // ADC 변환값 저장 변수 (ISR에서 수정됨)

// 4자리 7-segment에 숫자 출력 함수
void Segment(int N) {
	int i;
	unsigned char N1000, N100, N10, N1;
	int Buff;

	// 4자리 숫자 분리
	N1000 = N / 1000;
	Buff = N % 1000;
	N100 = Buff / 100;
	Buff = Buff % 100;
	N10 = Buff / 10;
	N1 = Buff % 10;

	// 각 자리별로 30번씩 빠르게 반복 출력 (잔상효과)
	for (i = 0; i < 30; i++) {
		// 1천 자리
		PORTC = 0x0E;
		PORTA = Font[N1000];
		_delay_ms(1);

		// 100 자리
		PORTC = 0x0D;
		PORTA = Font[N100];
		_delay_ms(1);

		// 10 자리
		PORTC = 0x0B;
		PORTA = Font[N10];
		_delay_ms(1);

		// 1 자리
		PORTC = 0x07;
		PORTA = Font[N1];
		_delay_ms(1);
	}
}

// ADC 변환 완료 인터럽트 서비스 루틴
ISR(ADC_vect) {
	adc_data = ADCW;            // ADC 변환 결과 읽기
	ADCSRA |= (1 << ADSC);      // 다음 변환 시작
}

// ADC 값에 따라 LED 점등 단계 조절 함수
void LED_Display(unsigned int adc_val) {
	/*
		ADC 값 범위: 0(밝음) ~ 650(어두움)
		밝을수록 LED 모두 꺼짐
		어두울수록 LED가 하나씩 켜짐
	*/

	const int steps = 8;              // LED 8개 사용 (PORTE 0~7)
	unsigned int threshold_min = 80;  // 이 값 이하면 LED 모두 켬 
	unsigned int threshold_max = 650; // 이 값 이상이면 LED 모두 끔 

	if (adc_val <= threshold_min) {
		PORTE = 0xFF;  // LED 모두 켬 (밝음)
		return;
	}

	if (adc_val >= threshold_max) {
		PORTE = 0x00;  // LED 모두 끔 (어두움)
		return;
	}

	// adc_val 이 threshold_min~threshold_max 범위 내에 있을 때,
	// 밝기가 낮아질수록 단계별로 LED가 꺼짐
	unsigned int level = (adc_val - threshold_min) * steps / (threshold_max - threshold_min);

	// level이 0이면 LED 모두 켜짐, level 7이면 1개만 켜짐
	unsigned char led_pattern = 0xFF << level;

	PORTE = led_pattern;
}

int main(void) {
	// 포트 설정
	DDRA = 0xFF;    // PORTA : 7-segment 숫자 출력 (출력)
	DDRC = 0x0F;    // PORTC : 7-segment 자리 선택 (하위 4비트 출력)
	PORTA = 0x00;
	PORTC = 0x0F;

	DDRE = 0xFF;    // PORTE : LED 8개 연결, 출력으로 설정
	PORTE = 0x00;   // 초기 LED 모두 끔

	DDRF &= ~(1 << PF3);  // PF3 (ADC3) 입력 설정
	PORTF &= ~(1 << PF3); // 풀업 저항 비활성화

	// ADC 설정
	ADMUX = (1 << REFS0) | 0x03;      // AVcc 기준, ADC3 채널 선택
	ADCSRA = (1 << ADEN)  |            // ADC 활성화
			 (1 << ADIE)  |            // ADC 변환 완료 인터럽트 활성화
			 (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 분주비 128

	_delay_us(10);                    // ADC 안정화 대기

	sei();                          // 전역 인터럽트 활성화
	ADCSRA |= (1 << ADSC);          // 첫 번째 ADC 변환 시작

	while (1) {
		Segment(adc_data);           // ADC 값을 7-segment에 출력
		LED_Display(adc_data);       // ADC 값에 따라 LED 점등 제어
	}
}
