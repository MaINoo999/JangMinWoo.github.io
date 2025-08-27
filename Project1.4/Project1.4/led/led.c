#include "led.h"

// LED 초기화 함수
void led_init(void) {
	// LED 핀을 모두 출력으로 설정
	LED_DDR |= ((1 << LED_RED_PIN) | (1 << LED_GREEN_PIN) | (1 << LED_BLUE_PIN));
	// 초기에는 모든 LED를 끈다 (Common Cathode 기준, HIGH로 출력)
	led_off();
}

// LED 색상 설정 함수
void led_set_color(unsigned char color_mask) {
	// Common Cathode 방식이므로, LED 핀이 LOW일 때 LED가 켜집니다.
	// 먼저 모든 LED 핀을 HIGH로 설정하여 끄기
	LED_PORT |= ((1 << LED_RED_PIN) | (1 << LED_GREEN_PIN) | (1 << LED_BLUE_PIN));

	// 주어진 color_mask에 해당하는 핀만 LOW로 설정하여 LED를 켬
	if (color_mask & (1 << LED_RED_PIN)) {
		LED_PORT &= ~(1 << LED_RED_PIN); // 빨간색 LED 켜기
	}
	if (color_mask & (1 << LED_GREEN_PIN)) {
		LED_PORT &= ~(1 << LED_GREEN_PIN); // 초록색 LED 켜기
	}
	if (color_mask & (1 << LED_BLUE_PIN)) {
		LED_PORT &= ~(1 << LED_BLUE_PIN); // 파란색 LED 켜기
	}
}

// 모든 LED 끄는 함수
void led_off(void) {
	// 모든 LED 핀을 HIGH로 설정하여 끄기 (Common Cathode 기준)
	LED_PORT |= ((1 << LED_RED_PIN) | (1 << LED_GREEN_PIN) | (1 << LED_BLUE_PIN));
}
