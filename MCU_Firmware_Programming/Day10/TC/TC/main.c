/*
 * TC.c
 *
 * Created: 2025-08-19 오후 3:17:08
 * Author : COMPUTER
 * Description:
 *   Timer/Counter1의 현재 값(TCNT1)을 읽어서
 *   4자리 7-Segment 디스플레이에 출력하는 예제입니다.
 *   - PORTA: 세그먼트 데이터 출력 (a~g)
 *   - PORTC: 자릿수 선택 (4자리)
 */

#define F_CPU 16000000UL    // 시스템 클럭 주파수 (16MHz)

#include <avr/io.h>         // I/O 포트 제어를 위한 헤더
#include <util/delay.h>     // _delay_ms 함수 사용

// 7-Segment에 출력할 폰트 데이터 (Common Cathode 기준)
// 0 ~ 9, A ~ F, '-' 및 '.' 포함
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
    0x08, // '-' (하이픈)
    0x80  // '.' (소수점)
};

/*
 * Segment 함수
 * - 입력값 N (0~9999)을 받아서 4자리로 분해하고
 *   7-Segment에 출력하는 함수입니다.
 * - N1000: 천의 자리, N100: 백의 자리
 * - N10: 십의 자리, N1: 일의 자리
 */
void Segment(int N) {
    int i;
    unsigned char N1000, N100, N10, N1;
    int Buff;

    // 자리수 분리
    N1000 = N / 1000;
    Buff = N % 1000;

    N100 = Buff / 100;
    Buff %= 100;

    N10 = Buff / 10;
    N1 = Buff % 10;

    // 4자리 반복해서 출력 (잔상 방지 및 가시성 확보)
    for (i = 0; i < 30; i++) {
        // 1번째 자리: 천의 자리
        PORTC = 0x0E;                 // PC0 LOW (나머지 HIGH)
        PORTA = Font[N1000];         // 폰트 출력
        _delay_ms(1);

        // 2번째 자리: 백의 자리
        PORTC = 0x0D;                 // PC1 LOW
        PORTA = Font[N100];
        _delay_ms(1);

        // 3번째 자리: 십의 자리
        PORTC = 0x0B;                 // PC2 LOW
        PORTA = Font[N10];
        _delay_ms(1);

        // 4번째 자리: 일의 자리
        PORTC = 0x07;                 // PC3 LOW
        PORTA = Font[N1];
        _delay_ms(1);
    }
}

/*
 * main 함수
 * - Timer/Counter1의 값(TCNT1)을 실시간으로 읽어서
 *   7-Segment에 표시합니다.
 */
int main(void) {
    unsigned int m_cnt = 0;

    // 포트 설정
    DDRA = 0xFF;   // PORTA: 세그먼트 숫자 출력용 → 출력 모드
    DDRC = 0x0F;   // PORTC: 세그먼트 자릿수 제어용 (PC0~PC3) → 출력 모드

    PORTA = 0xFF;  // PORTA 초기화 (모든 세그먼트 OFF)
    PORTC = 0x0F;  // PORTC 초기화 (모든 자릿수 OFF)

    // Timer1 설정
    // TCCR1B = 0x07 → 분주비 1024, Normal Mode (WGM13:0 = 0000)
    // 16MHz / 1024 = 약 15.6kHz → 1 tick ≈ 64us
    TCCR1B = 0x07;

    while (1) {
        m_cnt = TCNT1;  // 현재 Timer1 카운터 값 읽기 (0 ~ 65535)

        // 7-Segment는 최대 4자리(9999)까지 표시 가능
        if (m_cnt > 9999)
            m_cnt = 0;

        Segment(m_cnt);  // 세그먼트로 출력
    }
}
