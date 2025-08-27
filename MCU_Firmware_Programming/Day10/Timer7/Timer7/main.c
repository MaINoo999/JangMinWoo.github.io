/*
 * Timer7.c
 *
 * Created: 2025-08-19
 * Author : COMPUTER
 * Description: Timer1을 이용하여 1초 단위로 증가하는 숫자를 
 *              4자리 7-Segment에 표시하는 예제입니다.
 */

#define F_CPU 16000000UL    // 시스템 클럭 주파수 정의 (16MHz)

#include <avr/io.h>         // I/O 포트 관련 라이브러리
#include <avr/interrupt.h>  // 인터럽트 관련 라이브러리
#include <util/delay.h>     // 지연 함수 사용을 위한 라이브러리

// 7-Segment에 출력할 숫자(0~9) 및 A~F, 기타 문자의 폰트 배열 (Common Cathode 기준)
unsigned char Font[18] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7C, // 6
    0x07, // 7
    0x7F, // 8
    0x67, // 9
    0x77, // A
    0x7C, // b
    0x39, // C
    0x5E, // d
    0x79, // E
    0x71, // F
    0x08, // - (하이픈)
    0x80  // . (소수점)
};

// 카운터 변수
volatile int t_cnt = 0;  // 200ms 단위 카운트 (5번이면 1초)
volatile int m_cnt = 0;  // 실제로 1초 단위로 증가하는 값

// 7-Segment에 숫자를 표시하는 함수
void Segment(int N) {
    int i;
    unsigned char N1000, N100, N10, N1;
    int Buff;

    // 각 자리수 분해
    N1000 = N / 1000;          // 천의 자리
    Buff = N % 1000;
    N100 = Buff / 100;         // 백의 자리
    Buff %= 100;
    N10 = Buff / 10;           // 십의 자리
    N1 = Buff % 10;            // 일의 자리

    // 각 자리의 숫자를 세그먼트에 30번 반복 출력하여 충분히 밝게 보이게 함
    for (i = 0; i < 30; i++) {
        // 1번째 자리 (천의 자리)
        PORTC = 0x0E;                  // 첫 번째 자리 ON (PC0 LOW)
        PORTA = Font[N1000];          // 해당 자리 숫자 출력
        _delay_ms(1);

        // 2번째 자리 (백의 자리)
        PORTC = 0x0D;                  // 두 번째 자리 ON (PC1 LOW)
        PORTA = Font[N100];
        _delay_ms(1);

        // 3번째 자리 (십의 자리)
        PORTC = 0x0B;                  // 세 번째 자리 ON (PC2 LOW)
        PORTA = Font[N10];
        _delay_ms(1);

        // 4번째 자리 (일의 자리)
        PORTC = 0x07;                  // 네 번째 자리 ON (PC3 LOW)
        PORTA = Font[N1];
        _delay_ms(1);
    }
}

// 타이머1 오버플로우 인터럽트 서비스 루틴 (약 200ms 간격)
ISR(TIMER1_OVF_vect) {
    TCNT1 = 0xF4C0; // 오버플로우 발생 후 타이머 값 초기화 (16,256 - 4000 = 0xF4C0)
                    // 16MHz / 1024 = 15.625kHz → 1tick = 64us
                    // 4000 * 64us = 약 256ms (지연 약간 포함됨)

    t_cnt++;        // 200ms 단위 카운터 증가

    if (t_cnt >= 5) {
        t_cnt = 0;      // 200ms * 5 = 약 1초가 되면
        m_cnt++;        // 1초 카운터 증가

        if (m_cnt >= 10000) {
            m_cnt = 0;  // 4자리 세그먼트를 넘지 않도록 9999에서 0으로 리셋
        }
    }
}

int main(void) {
    // 포트 A: 세그먼트 숫자 출력용 (Font[] 사용)
    // 포트 C: 세그먼트 자리 선택용 (PC0 ~ PC3)
    DDRA = 0xFF;     // 포트 A를 출력으로 설정
    DDRC = 0xFF;     // 포트 C도 출력으로 설정

    PORTA = 0xFF;    // 초기값: 모든 세그먼트 꺼짐
    PORTC = 0xFF;    // 초기값: 모든 자릿수 꺼짐

    // 타이머1 설정
    TCCR1A = 0x00;                       // 일반 모드 (Normal Mode)
    TCCR1B = (1 << CS12) | (1 << CS10);  // 분주비 1024 (클럭 16MHz / 1024 = 약 15.6kHz)
    TCNT1 = 0xF4C0;                      // 초기 타이머 값 (16비트) 설정
    TIMSK = (1 << TOIE1);               // 타이머1 오버플로우 인터럽트 허용

    sei();                              // 전역 인터럽트 허용

    // 메인 루프: 1초 간격으로 증가하는 m_cnt 값을 세그먼트에 표시
    while (1) {
        Segment(m_cnt);  // 현재 m_cnt 값을 4자리 세그먼트에 표시
    }
}
