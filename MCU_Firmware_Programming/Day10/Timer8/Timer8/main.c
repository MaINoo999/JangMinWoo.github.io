/*
 * Timer8.c
 *
 * Created: 2025-08-19 오전 10:24:19
 * Author : COMPUTER
 * Description: Timer1을 이용한 Phase Correct PWM 모드로 RGB LED 중
 *              Red(PB5), Green(PB6)을 제어하고 색상을 순차적으로 출력.
 */

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

// RGB 색상 테이블 (Red, Green, Blue)
// Timer1은 PB5(R), PB6(G)만 가능하므로 PB7(B)는 실제 PWM 불가
unsigned char RGB_Table[5][3] = {
    { 163, 191, 64 },   // Yellowish Green
    { 255, 69, 0 },     // Orange Red
    { 34, 139, 34 },    // Forest Green
    { 135, 206, 235 },  // Sky Blue
    { 128, 0, 128 }     // Purple
};

int main(void) {
    unsigned char i;

    DDRB = 0xE0;  // PB5, PB6, PB7를 출력으로 설정
                  // 실질적으로 PB5(OC1A), PB6(OC1B)만 PWM 사용 가능

    ASSR = 0x00;  // 비동기 타이머 미사용, 내부 클럭 사용

    // Timer1 설정 (Phase Correct PWM, 8bit 모드)
    // WGM13:0 = 0001, COM1A1/COM1B1 = 1 (비반전 출력)
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);  // 0xA9
    TCCR1B = (1 << CS11);  // 분주비 8, 16MHz / 8 = 2MHz → 1tick = 0.5us
                           // 주기 = 256 * 2 (업다운) * 0.5us = 256us
    TCCR1C = 0x00;

    while (1) {
        for (i = 0; i < 5; i++) {
            OCR1AL = RGB_Table[i][0];  // PB5 (Red 채널) 듀티 설정
            OCR1BL = RGB_Table[i][1];  // PB6 (Green 채널) 듀티 설정

            // PB7은 타이머와 연결되지 않으므로 직접 ON/OFF 제어 가능 (소프트 PWM 필요 시 별도 구현)
            if (RGB_Table[i][2] > 127) {
                PORTB |= (1 << PB7);  // Blue ON
            } else {
                PORTB &= ~(1 << PB7); // Blue OFF
            }

            _delay_ms(1000); // 1초 간격으로 색상 변경
        }
    }
}
