/*
 * Timer5_Improved.c
 * Timer0 Fast PWM 모드 + OVF, COMP 인터럽트 사용
 * 2개의 2자리 7-seg 숫자 카운터 출력 (좌우 분리)
 *
 * Created: 2025-08-18 오후 4:14:37
 * Author : COMPUTER
 */

#define F_CPU 14745600UL  // CPU 클럭 주파수 정의 (14.7456 MHz)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// 7-seg 숫자 폰트 (공통 애노드 기준)
unsigned char Font[18] = {
    0x3F, 0x06, 0x5B, 0x4F,
    0x66, 0x6D, 0x7C, 0x07,
    0x7F, 0x67, 0x77, 0x7C,
    0x39, 0x5E, 0x79, 0x71,
    0x08, 0x80
};

// 타이머 카운터 변수
volatile int tr_cnt = 0, mr_cnt = 0; // 오른쪽 카운터: 타이머 오버플로우 기준
volatile int tl_cnt = 0, ml_cnt = 0; // 왼쪽 카운터: 타이머 출력 비교 기준

// 오른쪽 2자리 7-seg 출력 함수
void RSegment(int N)
{
    int i;
    unsigned char N10 = N / 10;
    unsigned char N1 = N % 10;

    for (i = 0; i < 5; i++) {
        PORTC = 0x0B;      // 오른쪽 3번째 자리 선택 (PC3=0, 나머지 1)
        PORTA = Font[N10]; // 십의 자리 출력
        _delay_ms(1);

        PORTC = 0x07;      // 오른쪽 4번째 자리 선택 (PC0=0)
        PORTA = Font[N1];  // 일의 자리 출력
        _delay_ms(1);
    }
}

// 왼쪽 2자리 7-seg 출력 함수
void LSegment(int N)
{
    int i;
    unsigned char N10 = N / 10;
    unsigned char N1 = N % 10;

    for (i = 0; i < 5; i++) {
        PORTC = 0x0E;      // 왼쪽 1번째 자리 선택 (PC1=0)
        PORTA = Font[N10]; // 십의 자리 출력
        _delay_ms(1);

        PORTC = 0x0D;      // 왼쪽 2번째 자리 선택 (PC2=0)
        PORTA = Font[N1];  // 일의 자리 출력
        _delay_ms(1);
    }
}

// Timer0 Overflow 인터럽트 서비스 루틴
ISR(TIMER0_OVF_vect)
{
    tr_cnt++;
    if (tr_cnt >= 56) {   // 약 56 * 0.07ms ≈ 4ms 간격
        tr_cnt = 0;
        mr_cnt++;         // 오른쪽 카운터 증가
        if (mr_cnt > 99)  // 2자리 제한
            mr_cnt = 0;
    }
}

// Timer0 Output Compare 인터럽트 서비스 루틴
ISR(TIMER0_COMP_vect)
{
    tl_cnt++;
    if (tl_cnt >= 28) {   // 약 28 * 0.07ms ≈ 2ms 간격
        tl_cnt = 0;
        ml_cnt++;         // 왼쪽 카운터 증가
        if (ml_cnt > 99)  // 2자리 제한
            ml_cnt = 0;
    }
}

int main(void)
{
    // 포트 설정
    DDRA = 0xFF;    // PORTA: 7-seg 데이터 출력 (출력)
    DDRC = 0x0F;    // PORTC 하위 4비트: 7-seg 자리 선택 출력
    PORTA = 0xFF;   // 초기값 - 모두 소등 (공통 애노드 기준)
    PORTC = 0x0F;   // 자리 선택 모두 비활성화 (1로 셋팅)

    DDRB = 0x10;    // PB4 (OC0) 출력 설정

    ASSR = 0;       // 내부 클럭 사용

    /*
    Timer0 설정
    Fast PWM 모드 (WGM01=1, WGM00=1), 비반전 출력 (COM01=1, COM00=0)
    분주비 1024 (CS02=1, CS01=0, CS00=1)
    PWM 주파수 계산:
      F_CPU = 14.7456MHz
      분주 후 = 14,745,600 / 1024 ≈ 14,400 Hz
      PWM 주기 = 1 / 14,400 ≈ 69.44 us
    */
    TCCR0 = 0x6F;  // 0b01101111: COM01=1, WGM01=1, WGM00=1, CS02=1, CS00=1

    TIMSK = (1 << TOIE0) | (1 << OCIE0);  // 오버플로우 및 출력 비교 인터럽트 허용

    TCNT0 = 0x00;  // 카운터 초기화
    OCR0 = 102;    // 출력 비교값 설정 (PWM 듀티 사이클 제어)

    TIFR = 0x00;   // 인터럽트 플래그 초기화

    sei();        // 전역 인터럽트 허용

    while (1) {
        // 좌우 7-seg에 카운트 출력 (2자리씩)
        LSegment(ml_cnt);
        RSegment(mr_cnt);
    }
}
