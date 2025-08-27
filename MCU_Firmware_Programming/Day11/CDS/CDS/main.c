/*
 * CDS + Relay.c
 *
 * Created: 2025-08-20
 * Author : COMPUTER
 *
 * Description:
 * - CDS 조도센서(PF3)를 통해 아날로그 값을 ADC로 읽어
 *   7-segment(4자리)에 표시
 * - 릴레이(PB0)를 통해 팬을 5초 ON, 5초 OFF 반복 제어
 */

#define F_CPU 16000000UL  // 시스템 클럭 주파수 설정 (16MHz)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// 7-Segment 숫자 폰트 테이블 (공통 애노드 기준)
// Font[숫자] = 출력할 세그먼트 패턴
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

// ADC 변환된 데이터 저장용 변수 (인터럽트에서 업데이트)
volatile unsigned int adc_data = 0;

// 7-Segment에 4자리 숫자 출력하는 함수
void Segment(int N) {
	// 4자리 숫자 분리: 천의 자리, 백의 자리, 십의 자리, 일의 자리
	unsigned char N1000 = N / 1000;
	unsigned char N100 = (N % 1000) / 100;
	unsigned char N10  = (N % 100) / 10;
	unsigned char N1   = N % 10;

	// 잔상 방지 + 눈에 보이게 반복 출력
	for (int i = 0; i < 80; i++) {
		// 1. 천의 자리 표시
		PORTC = 0x0E;                  // 1110 → 첫 번째 자리 선택
		PORTA = Font[N1000];           // 천의 자리 숫자 출력
		_delay_ms(5);

		// 2. 백의 자리 표시
		PORTC = 0x0D;                  // 1101 → 두 번째 자리 선택
		PORTA = Font[N100];
		_delay_ms(5);

		// 3. 십의 자리 표시
		PORTC = 0x0B;                  // 1011 → 세 번째 자리 선택
		PORTA = Font[N10];
		_delay_ms(5);

		// 4. 일의 자리 표시
		PORTC = 0x07;                  // 0111 → 네 번째 자리 선택
		PORTA = Font[N1];
		_delay_ms(5);
	}
}

// ADC 변환 완료 인터럽트 함수
ISR(ADC_vect) {
	adc_data = ADCW;             // 변환된 아날로그 값을 읽어서 저장
	ADCSRA |= (1 << ADSC);       // 다음 ADC 변환 시작
}

int main(void) {
	// ----------------------------------
	// 1. 포트 초기화
	// ----------------------------------
	DDRA = 0xFF;        // PORTA (7-Segment 출력) → 출력 모드 설정
	DDRC = 0x0F;        // PORTC (자리 선택용) 하위 4비트 출력
	PORTA = 0x00;       // PORTA 초기화
	PORTC = 0x0F;       // PORTC 자리 모두 OFF 상태

	DDRB |= (1 << PB0);     // PB0 릴레이 제어용 → 출력 설정
	PORTB &= ~(1 << PB0);   // 릴레이 초기 OFF 상태

	// ----------------------------------
	// 2. ADC 설정
	// ----------------------------------
	DDRF &= ~(1 << PF3);    // PF3(ADC3 채널) 입력 설정
	PORTF &= ~(1 << PF3);   // 풀업 저항 비활성화

	ADMUX = (1 << REFS0) | 0x03;
	// REFS0 = 1 → AVcc (5V) 기준 전압 사용
	// MUX = 00011 → ADC3 채널 선택 (PF3)

	ADCSRA = (1 << ADEN)  |    // ADC 기능 활성화
	         (1 << ADIE)  |    // ADC 변환 완료 인터럽트 허용
	         (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	// 분주비 128 → 16MHz / 128 = 125kHz (권장 ADC 클럭)

	_delay_us(10);         // 안정화 시간

	sei();                 // 전역 인터럽트 활성화

	ADCSRA |= (1 << ADSC); // 첫 번째 변환 시작

	// ----------------------------------
	// 3. 메인 루프
	// ----------------------------------
	while (1) {
		// [1] 조도센서 값 표시
		Segment(adc_data);

		// [2] 릴레이 (FAN) 제어
		PORTB |= (1 << PB0);     // PB0 = HIGH → 릴레이 ON
		_delay_ms(5000);         // 5초 동안 동작

		PORTB &= ~(1 << PB0);    // PB0 = LOW → 릴레이 OFF
		_delay_ms(1000);         // 1초 정지
	}
}
