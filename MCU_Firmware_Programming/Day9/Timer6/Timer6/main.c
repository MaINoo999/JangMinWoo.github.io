/*
 * Timer6.c
 *
 * Created: 2025-08-18 오후 4:21:38
 * Author : COMPUTER
 */

#define F_CPU 14745600UL    // CPU 클럭 주파수 14.7456 MHz (본인 환경에 맞게 조정)

// AVR 표준 헤더
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// 7-세그먼트에 표시할 숫자별 LED 패턴 (공통 애노드 기준)
unsigned char Font[18] = {
    0x3F, 0x06, 0x5B, 0x4F,
    0x66, 0x6D, 0x7C, 0x07,
    0x7F, 0x67, 0x77, 0x7C,
    0x39, 0x5E, 0x79, 0x71,
    0x08, 0x80
};

// 오른쪽 숫자 카운터 변수
volatile int tr_cnt = 0;
volatile int mr_cnt = 0;

// 왼쪽 숫자 카운터 변수
volatile int tl_cnt = 0;
volatile int ml_cnt = 0;

// 오른쪽 2자리 7-세그먼트 출력 함수
void RSegment(int N)
{
    int i;
    unsigned char N10 = N / 10;  // 십의 자리
    unsigned char N1 = N % 10;   // 일의 자리

    for (i = 0; i < 5; i++) {
        PORTC = 0x0B;          // 오른쪽 3번째 자리 활성화 (PC3=0)
        PORTA = Font[N10];     // 십의 자리 출력
        _delay_ms(1);

        PORTC = 0x07;          // 오른쪽 4번째 자리 활성화 (PC0=0)
        PORTA = Font[N1];      // 일의 자리 출력
        _delay_ms(1);
    }
}

// 왼쪽 2자리 7-세그먼트 출력 함수
void LSegment(int N)
{
    int i;
    unsigned char N10 = N / 10;  // 십의 자리
    unsigned char N1 = N % 10;   // 일의 자리

    for (i = 0; i < 5; i++) {
        PORTC = 0x0E;          // 왼쪽 1번째 자리 활성화 (PC1=0)
        PORTA = Font[N10];     // 십의 자리 출력
        _delay_ms(1);

        PORTC = 0x0D;          // 왼쪽 2번째 자리 활성화 (PC2=0)
        PORTA = Font[N1];      // 일의 자리 출력
        _delay_ms(1);
    }
}

// Timer0 Overflow 인터럽트 서비스 루틴
ISR(TIMER0_OVF_vect)
{
    tr_cnt++;
    if (tr_cnt >= 56) {    // 약 56 * 0.07ms = 약 4ms 간격으로 카운트 증가
        tr_cnt = 0;
        mr_cnt++;          // 오른쪽 카운터 증가
        if (mr_cnt > 99)   // 99 초과 시 0으로 초기화
            mr_cnt = 0;
    }
}

// Timer0 Output Compare 인터럽트 서비스 루틴
ISR(TIMER0_COMP_vect)
{
    tl_cnt++;
    if (tl_cnt >= 28) {    // 약 28 * 0.07ms = 약 2ms 간격으로 카운트 증가
        tl_cnt = 0;
        ml_cnt++;          // 왼쪽 카운터 증가
        if (ml_cnt > 99)   // 99 초과 시 0으로 초기화
            ml_cnt = 0;
    }
}

int main(void)
{
    // 포트 초기화
    DDRA = 0xFF;      // PORTA : 7-세그먼트 LED 출력 (모두 출력 설정)
    DDRC = 0x0F;      // PORTC 하위 4비트: 7-세그먼트 자리 선택 (출력)
    PORTA = 0xFF;     // PORTA 초기값 (LED OFF, 공통 애노드)
    PORTC = 0x0F;     // PORTC 초기값 (자리 모두 비활성화)

    DDRB = 0x10;      // PB4 (OC0) 출력 설정

    ASSR = 0;         // Timer0 내부 클럭 사용

    /*
    Timer0 설정:
    - Fast PWM 모드 (WGM00=1, WGM01=1)
    - 비반전 모드 (COM01=1, COM00=0)
    - 분주비 1024 (CS02=1, CS01=0, CS00=1)
    */
    TCCR0 = 0x6F;     // 0b01101111

    TIMSK = (1 << TOIE0) | (1 << OCIE0);  // Timer0 Overflow 및 Output Compare 인터럽트 허용

    TCNT0 = 0x00;     // Timer0 초기 카운터 값
    OCR0 = 102;       // Output Compare 레지스터 설정 (PWM 듀티 사이클 조정)

    TIFR = 0x00;      // 인터럽트 플래그 초기화

    sei();            // 전역 인터럽트 활성화

    while (1)
    {
        LSegment(ml_cnt);   // 왼쪽 숫자 표시
        RSegment(mr_cnt);   // 오른쪽 숫자 표시
    }
}
