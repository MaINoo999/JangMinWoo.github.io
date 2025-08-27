#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// 7세그먼트 폰트 (0~9, A~F 등 일부 문자 포함)
unsigned char Font[18] = {
	0x3F, 0x06, 0x5B, 0x4F,
	0x66, 0x6D, 0x7C, 0x07,
	0x7F, 0x67, 0x77, 0x7C,
	0x39, 0x5E, 0x79, 0x71,
	0x08, 0x80
};

volatile unsigned int adc_data = 0;   // ADC 변환 결과 저장 변수 (인터럽트 내 업데이트)

// LED 밝기 배열 (각 LED 밝기 값, 0~255)
unsigned char led_brightness[8] = {0};

// 4자리 7세그먼트 숫자 출력 함수
void Segment(int N) {
	unsigned char N1000 = N / 1000;
	int buff = N % 1000;
	unsigned char N100 = buff / 100;
	buff = buff % 100;
	unsigned char N10 = buff / 10;
	unsigned char N1 = buff % 10;

	// 각 자리수별로 빠르게 순차 출력 (멀티플렉싱)
	for (int i = 0; i < 30; i++) {
		PORTC = 0x0E; PORTA = Font[N1000]; _delay_ms(1);
		PORTC = 0x0D; PORTA = Font[N100];  _delay_ms(1);
		PORTC = 0x0B; PORTA = Font[N10];   _delay_ms(1);
		PORTC = 0x07; PORTA = Font[N1];    _delay_ms(1);
	}
}

// ADC 변환 완료 인터럽트 서비스 루틴
ISR(ADC_vect) {
	adc_data = ADCW;         // ADC 결과 저장
	ADCSRA |= (1 << ADSC);  // 다음 ADC 변환 시작
}

// LED 밝기를 ADC 값에 따라 4단계로 설정하는 함수
void Set_LED_Brightness(unsigned int adc_val) {
	const unsigned int threshold_min = 0;
	const unsigned int threshold_max = 700;

	if (adc_val < threshold_min) adc_val = threshold_min;
	if (adc_val > threshold_max) adc_val = threshold_max;

	// 전체 밝기 역비례 계산 (0~255)
	unsigned char overall_brightness = (unsigned char)((threshold_max - adc_val) * 255 / (threshold_max - threshold_min));

	// 6단계 밝기 값 배열 (0%, 20%, 40%, 60%, 80%, 100%)
	const unsigned char brightness_levels[6] = {0, 51, 102, 153, 204, 255};

	for (int i = 0; i < 8; i++) {
		unsigned int raw_level = ((i + 1) * overall_brightness) / 8;

		unsigned char quantized_level = 0;
		unsigned char min_diff = 255;

		// 가장 가까운 6단계 밝기 찾기
		for (int lvl = 0; lvl < 6; lvl++) {
			unsigned char diff = (raw_level > brightness_levels[lvl]) ? (raw_level - brightness_levels[lvl]) : (brightness_levels[lvl] - raw_level);
			if (diff < min_diff) {
				min_diff = diff;
				quantized_level = brightness_levels[lvl];
			}
		}

		led_brightness[i] = quantized_level;
	}
}


// 소프트웨어 PWM 함수: pwm_counter와 led_brightness 비교해 LED ON/OFF 조절
void LED_SoftwarePWM(void) {
	static unsigned char pwm_counter = 0;
	unsigned char port_value = 0;

	for (int i = 0; i < 8; i++) {
		// pwm_counter가 led_brightness[i]보다 작으면 LED ON
		if (pwm_counter < led_brightness[i]) {
			port_value |= (1 << i);
		}
	}

	PORTE = port_value;  // LED 출력 포트에 값 쓰기
	pwm_counter++;       // 카운터 증가 (0~255 자동 오버플로우)
}

int main(void) {
	// 포트 초기화
	DDRA = 0xFF;    // 7세그먼트 데이터 출력 (포트 A)
	DDRC = 0x0F;    // 7세그먼트 자리 선택 (포트 C 하위 4비트)
	PORTA = 0x00;
	PORTC = 0x0F;

	DDRE = 0xFF;    // LED 출력 (포트 E)
	PORTE = 0x00;

	// ADC 입력 핀 (PF3) 설정
	DDRF &= ~(1 << PF3);  // 입력으로 설정
	PORTF &= ~(1 << PF3); // 풀업 저항 비활성화

	// ADC 설정: AVcc 기준, ADC3 (PF3) 입력 채널 선택
	ADMUX = (1 << REFS0) | 0x03;

	// ADC 활성화, 인터럽트 허용, 분주비 128 (16MHz/128=125kHz)
	ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

	_delay_us(10);

	sei();              // 전역 인터럽트 활성화
	ADCSRA |= (1 << ADSC); // ADC 변환 시작

	while (1) {
		// ADC 값에 따라 LED 밝기 단계 설정
		Set_LED_Brightness(adc_data);

		// ADC 값을 7세그먼트로 표시
		Segment(adc_data);

		// 소프트웨어 PWM으로 LED 밝기 제어 (50 사이클, 약 5ms)
		for (int i = 0; i < 50; i++) {
			LED_SoftwarePWM();
			_delay_us(100);
		}
	}
}
