/*
 * VRES.c
 *
 * Created: 2025-08-20 오전 11:26:46
 * Author : COMPUTER
 */ 

#define F_CPU 16000000UL	

#include <util/delay.h>
#include <avr/io.h>

// 7-seg Font 배열 (0~9 및 몇몇 문자)
unsigned char Font[18] = {
	0x3F, 0x06, 0x5B, 0x4F,
	0x66, 0x6D, 0x7C, 0x07,
	0x7F, 0x67, 0x77, 0x7C,
	0x39, 0x5E, 0x79, 0x71,
	0x08, 0x80
};

void Segment(int N);
unsigned int adc_data= 0;

// 7-segment 4자리 출력 함수
void Segment(int N){
	int i;
	unsigned char N1000, N100, N10, N1;
	int Buff;
	
	N1000 = N/1000;
	Buff = N%1000;
	
	N100 = Buff/100;
	Buff = Buff%100;
	
	N10 = Buff/10;
	N1 = Buff%10;
	
	for(i=0; i<30; i++){
		PORTC = 0x0E;	// 첫 번째 자리 선택 (예: 1110)
		PORTA = Font[N1000];
		_delay_ms(1);
		
		PORTC = 0x0D;	// 두 번째 자리 선택 (예: 1101)
		PORTA = Font[N100];
		_delay_ms(1);
		
		PORTC = 0x0B;	// 세 번째 자리 선택 (예: 1011)
		PORTA = Font[N10];
		_delay_ms(1);
		
		PORTC = 0x07;	// 네 번째 자리 선택 (예: 0111)
		PORTA = Font[N1];
		_delay_ms(1);
	}
}

// ADC값 읽기 함수
unsigned int read_adc(void){
	ADCSRA |= (1 << ADSC);	// ADC 변환 시작
	
	while(ADCSRA & (1 << ADSC)); // 변환 완료까지 대기
	
	return ADCW;	// ADC 변환 결과 반환
}

int main(void){
	// PORTA: 7-seg segment 출력 (출력)
	DDRA = 0xFF;
	PORTA = 0x00;
	
	// PORTC: 7-seg 자리 선택 출력 (출력)
	DDRC = 0x0F;	// 하위 4비트만 출력으로 설정
	PORTC = 0x0F;	// 초기값 설정
	
	// ADC 초기화 (AVcc 기준, 채널0, 분주비 128)
	ADMUX = (1 << REFS0);			// AVcc를 기준 전압으로 설정, ADC0 선택
	ADCSRA = (1 << ADEN) |			// ADC 활성화
			 (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	// 분주비 128 (16MHz/128 = 125kHz)
	
	while(1){
		adc_data = read_adc();	// ADC 데이터 읽기 (0~1023)
		
		// ADC값을 0~9999 범위로 변환해서 출력 (원한다면 수정 가능)
		// 여기서는 0~1023 범위를 그대로 0~1023 출력 (4자리)
		Segment(adc_data);
	}
	
	return 0;
}
