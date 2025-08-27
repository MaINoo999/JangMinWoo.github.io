/*
 * Timer2_CTC_Improved.c
 * Timer2 출력비교 인터럽트를 이용한 7-Segment 카운터
 * 2025-08-18
 */

#define F_CPU 14745600UL  // 시스템 클럭 (14.7456MHz 기준)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// 7-Segment 숫자 폰트 (공통 애노드 기준)
const unsigned char Font[18] = {
    0x3F, 0x06, 0x5B, 0x4F,
    0x66, 0x6D, 0x7C, 0x07,
    0x7F, 0x67, 0x77, 0x7C,
    0x39, 0x5E, 0x79, 0x71,
    0x08, 0x80
};

// 전역 변수
volatile int t_cnt = 0, m_cnt = 0;

// 4자리 7-세그먼트 표시 함수
void Segment(int N)
{
    int i;
    unsigned char N1000, N100, N10, N1;
    int Buff;

    N1000 = N / 1000;
    Buff = N % 1000;
    N100 = Buff / 100;
    Buff %= 100;
    N10 = Buff / 10;
    N1 = Buff % 10;

    for(i = 0; i < 30; i++) {
        PORTC = 0x0E;  // 첫째 자리 ON
        PORTA = Font[N1000];
        _delay_ms(1);

        PORTC = 0x0D;  // 둘째 자리 ON
        PORTA = Font[N100];
        _delay_ms(1);

        PORTC = 0x0B;  // 셋째 자리 ON
        PORTA = Font[N10];
        _delay_ms(1);

        PORTC = 0x07;  // 넷째 자리 ON
        PORTA = Font[N1];
        _delay_ms(1);
    }
}

// Timer2 출력비교 인터럽트 (CTC 모드)
ISR(TIMER2_COMP_vect)
{
    t_cnt++;
    if(t_cnt >= 100) {   // 100 * 10ms = 1초
        t_cnt = 0;
        m_cnt++;
        if(m_cnt > 9999)  // 4자리 범위 초과 시 초기화
            m_cnt = 0;
    }
}

int main(void)
{
    // 세그먼트 제어 포트 설정
    DDRA = 0xFF;  // 세그먼트 데이터 포트
    DDRC = 0x0F;  // 세그먼트 자릿수 선택 포트
    PORTA = 0xFF; // 초기값
    PORTC = 0x00;

    // Timer2 설정 (CTC 모드, 분주비 1024)
    TCCR2 = (1 << WGM21) | (1 << CS22) | (1 << CS21) | (1 << CS20); // CTC + 1024 분주
    OCR2 = 144;     // 출력 비교 값 설정 (14.7456MHz / 1024 = 약 14.4KHz → 144 → 약 10ms)
    TCNT2 = 0;      // 타이머 초기값
    TIMSK |= (1 << OCIE2);  // 출력 비교 인터럽트 허용

    sei();  // 전역 인터럽트 허용

    while (1) {
        Segment(m_cnt);  // 현재 카운트 표시
    }
}
