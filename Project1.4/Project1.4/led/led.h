#ifndef LED_H_
#define LED_H_

#define F_CPU 14745600UL // 클럭 주파수 정의

#include <avr/io.h>
#include <util/delay.h>

// 풀컬러 LED 핀 정의 (연결 가이드에 맞게 설정)
#define LED_PORT    PORTE   // LED 포트 (컬러 핀들을 연결할 포트)
#define LED_DDR     DDRE    // LED 핀의 입출력 방향 설정

// LED의 각 색상에 해당하는 핀
#define LED_RED_PIN     PE0
#define LED_GREEN_PIN   PE1
#define LED_BLUE_PIN    PE2

// 색상 정의 (Common Cathode 기준: 해당 핀이 LOW일 때 LED가 켜짐)
#define LED_OFF     0x00 // 모든 LED 끄기
#define LED_RED     (1 << LED_RED_PIN)
#define LED_GREEN   (1 << LED_GREEN_PIN)
#define LED_BLUE    (1 << LED_BLUE_PIN)
#define LED_YELLOW  ((1 << LED_RED_PIN) | (1 << LED_GREEN_PIN)) // Red + Green
#define LED_CYAN    ((1 << LED_GREEN_PIN) | (1 << LED_BLUE_PIN)) // Green + Blue
#define LED_MAGENTA ((1 << LED_RED_PIN) | (1 << LED_BLUE_PIN)) // Red + Blue
#define LED_WHITE   ((1 << LED_RED_PIN) | (1 << LED_GREEN_PIN) | (1 << LED_BLUE_PIN)) // Red + Green + Blue

void led_init(void); // LED 초기화 함수
void led_set_color(unsigned char color_mask); // LED 색상 설정 함수
void led_off(void); // 모든 LED 끄는 함수

#endif /* LED_H_ */
