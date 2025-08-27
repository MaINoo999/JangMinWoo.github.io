/*
 * AnalogWrite.c
 * Created: 2025-08-20
 * Author : COMPUTER
 * Description: ATmega128 + LED + 버튼 → 소프트웨어 PWM으로 밝기 조절
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

// 8개 LED 각각 밝기 값을 저장하는 배열 (0 ~ 255)
uint8_t brightness[8] = {0,0,0,0,0,0,0,0};

// PWM 카운터 변수 (0 ~ 255)
uint8_t pwm_step = 0;

// 각 버튼의 이전 눌림 상태 저장 (true: 눌림, false: 안눌림)
// 버튼 눌림 이벤트 감지를 위해 필요
bool buttonPrevState[8] = {false,false,false,false,false,false,false,false};

// 버튼 눌림 여부 확인 함수 (active low, 내부 풀업 사용)
// 눌렸으면 true, 안눌렸으면 false 반환
bool isButtonPressed(uint8_t pin) {
	return !(PINC & (1 << pin));
}

int main(void) {
	// PORTA 전체를 출력으로 설정 (LED 연결)
	DDRA = 0xFF;
	PORTA = 0x00;  // LED 초기 모두 끔

	// PORTC 전체를 입력으로 설정 (버튼 연결)
	DDRC = 0x00;
	PORTC = 0xFF;  // 내부 풀업 저항 활성화 (버튼 미눌림 시 입력은 HIGH)

	while (1) {
		// 1. 소프트웨어 PWM 출력 처리
		// pwm_step가 0부터 255까지 증가하며,
		// 각 LED 밝기 값과 비교해 ON/OFF 결정
		for (uint8_t i=0; i<8; i++) {
			if (brightness[i] > pwm_step)
				PORTA |= (1 << i);   // pwm_step보다 밝기가 크면 LED 켜기
			else
				PORTA &= ~(1 << i);  // 아니면 LED 끄기
		}

		// PWM 카운터 증가 및 255 넘으면 0으로 리셋
		pwm_step++;
		if (pwm_step == 255) pwm_step = 0;

		// 2. 버튼 상태 확인 및 밝기 조절
		// 각 버튼별로 눌림 이벤트(이전 상태는 안눌림, 현재 눌림) 발생 시
		// 해당 LED 밝기를 32만큼 증가시킨 후 255 넘으면 0으로 초기화
		for (uint8_t i=0; i<8; i++) {
			bool pressed = isButtonPressed(i);

			// 눌림 이벤트 감지: 이전에는 안눌림이고 현재 눌림일 때만 작동
			if (pressed && !buttonPrevState[i]) {
				brightness[i] += 32;     // 밝기 단계 증가
				if (brightness[i] > 255)
					brightness[i] = 0;   // 최대값 넘으면 0으로 초기화
			}

			// 현재 버튼 상태를 이전 상태 배열에 저장하여
			// 다음 반복 때 눌림 이벤트 감지에 활용
			buttonPrevState[i] = pressed;
		}
	}
}
