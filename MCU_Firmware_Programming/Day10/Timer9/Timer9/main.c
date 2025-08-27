/*
 * RGB + 7-Segment Timer 통합 예제
 * Created: 2025-08-19
 * Author : COMPUTER
 *
 * - Timer1: Phase Correct PWM (8bit) 모드 → PB5(R), PB6(G)에 RGB 출력
 * - Timer3: Overflow Interrupt 이용 → 1초마다 카운트 증가 (7세그먼트 표시)
 */

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// RGB 색상 테이블 (Red, Green, Blue)
unsigned char RGB_Table[5][3] = {
    { 163, 191, 64 },   // Yellowish Green
    { 255, 69, 0 },     // Orange Red
    { 34, 139, 34 },    // Forest Green
    { 135, 206, 235 },  // Sky Blue
    { 128, 0, 128 }     // Purple
};

// 7-Segment 표시용 숫자 폰트 (Common Cathode 기준)
unsigned char Font[18] = {
    0x3F, 0x06, 0x5B, 0x4F,
    0x66, 0x6D, 0x7C, 0x07,
    0x7F, 0x67, 0x77, 0x7C,
    0x39, 0x5E, 0x79, 0x71,
    0x08, 0x80
};

// 글로벌 변수
volatile int t_cnt = 0;    // Timer3: 200ms 단위 카운터
volatile int m_cnt = 0;    // 1초마다 증가하는 숫자
unsigned char color_index = 0;  // RGB 색상 인덱스

// --------------------------
// 7-Segment 출력 함수
// --------------------------
void Segment(int N) {
    int i;
    unsigned char N1000, N100, N10, N1;
    int Buff;

    // 각 자리수 추출
    N1000 = N / 1000;
    Buff = N % 1000;
    N100 = Buff / 100;
    Buff %= 100;
    N10 = Buff / 10;
    N1 = Buff % 10;

    for (i = 0; i < 30; i++) {
        PORTC = 0x0E;        // 첫 번째 자리 ON (PC0)
        PORTA = Font[N1000]; _delay_ms(1);

        PORTC = 0x0D;        // 두 번째 자리 ON (PC1)
        PORTA = Font[N100];  _delay_ms(1);

        PORTC = 0x0B;        // 세 번째 자리 ON (PC2)
        PORTA = Font[N10];   _delay_ms(1);

        PORTC = 0x07;        // 네 번째 자리 ON (PC3)
        PORTA = Font[N1];    _delay_ms(1);
    }
}

// --------------------------
// Timer3 오버플로우 인터럽트
// (약 200ms 주기로 발생)
// --------------------------
ISR(TIMER3_OVF_vect) {
    TCNT3 = 0xF4C0;   // 초기값 재설정 (4000 tick)

    t_cnt++;
    if (t_cnt >= 5) {   // 약 1초 경과
        t_cnt = 0;
        m_cnt++;

        if (m_cnt >= 10000) {
            m_cnt = 0;  // 4자리 넘으면 0으로 리셋
        }

        // 색상도 함께 변경 (1초마다)
        color_index++;
        if (color_index >= 5) {
            color_index = 0;
        }

        // PWM 듀티 변경 (RGB)
        OCR1AL = RGB_Table[color_index][0];  // Red - PB5
        OCR1BL = RGB_Table[color_index][1];  // Green - PB6

        // Blue 채널은 PB7 소프트 제어
        if (RGB_Table[color_index][2] > 127)
            PORTB |= (1 << PB7);   // ON
        else
            PORTB &= ~(1 << PB7);  // OFF
    }
}

// --------------------------
// 메인 함수
// --------------------------
int main(void) {
    // ---------------------
    // 포트 설정
    // ---------------------
    DDRA = 0xFF;  // 세그먼트 숫자 출력 (Font)
    DDRC = 0xFF;  // 세그먼트 자릿수 제어
    DDRB = 0xE0;  // PB5~PB7 출력 (RGB)

    PORTA = 0xFF;
    PORTC = 0xFF;
    PORTB = 0x00;

    // ---------------------
    // Timer1: PWM 설정 (RGB)
    // ---------------------
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10); // 비반전, 8bit Phase Correct
    TCCR1B = (1 << CS11);  // 분주비 8 → 16MHz / 8 = 2MHz
    TCCR1C = 0x00;

    OCR1AL = RGB_Table[0][0];  // 초기 Red 듀티
    OCR1BL = RGB_Table[0][1];  // 초기 Green 듀티

    // ---------------------
    // Timer3: 세그먼트용 타이머 설정 (1초 카운트)
    // ---------------------
    TCCR3A = 0x00;                        // Normal mode
    TCCR3B = (1 << CS32) | (1 << CS30);   // 분주비 1024
    TCNT3 = 0xF4C0;                       // 4000 tick → 약 256ms (200ms 근사)
    ETIMSK = (1 << TOIE3);               // Timer3 Overflow 인터럽트 허용

    sei();  // 전역 인터럽트 허용

    // ---------------------
    // 메인 루프
    // ---------------------
    while (1) {
        Segment(m_cnt);  // 세그먼트에 현재 값 표시
    }
}
