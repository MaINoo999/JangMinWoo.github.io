/*
 * TC2.c
 *
 * Created: 2025-08-19 오후 3:38:38
 * Author : COMPUTER
 * Description:
 *   Timer1의 입력 캡처 기능을 이용해 외부 펄스의 시간 간격(ICR1)을 측정하고,
 *   해당 값을 7-Segment에 표시하는 예제입니다.
 */

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> // 인터럽트 사용을 위한 헤더

// 7-Segment Font 배열 (Common Cathode 기준)
unsigned char Font[18] = {
    0x3F, 0X06, 0X5B, 0X4F,
    0X66, 0X6D, 0X7C, 0X07,
    0X7F, 0X67, 0X77, 0X7C,
    0X39, 0X5E, 0X79, 0X71,
    0X08, 0X80
};

// 캡처된 카운트 값을 저장할 변수 (입력 주기 측정 결과)
volatile unsigned int m_cnt = 0;

/*
 * Segment 함수
 * - 전달된 4자리 숫자(int N)를 7-Segment에 출력
 * - PORTA: 데이터 (세그먼트 a~g), PORTC: 자릿수 선택 (4개)
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

    // 4자리 스캔 반복 (잔상 제거 및 시인성 확보)
    for (i = 0; i < 30; i++) {
        PORTC = 0x0E; // 천의 자리 선택 (PC0 LOW)
        PORTA = Font[N1000];
        _delay_ms(1);

        PORTC = 0x0D; // 백의 자리 선택 (PC1 LOW)
        PORTA = Font[N100];
        _delay_ms(1);

        PORTC = 0x0B; // 십의 자리 선택 (PC2 LOW)
        PORTA = Font[N10];
        _delay_ms(1);

        PORTC = 0x07; // 일의 자리 선택 (PC3 LOW)
        PORTA = Font[N1];
        _delay_ms(1);
    }
}

/*
 * Timer1 입력 캡처 인터럽트
 * - 외부에서 상승에지(↑)가 발생하면 ICR1에 현재 타이머 값 저장됨
 * - 해당 값을 m_cnt에 저장하여 세그먼트에 표시할 수 있게 함
 */
ISR(TIMER1_CAPT_vect) {
    m_cnt = ICR1;  // ICR1 레지스터의 값 → 외부 펄스 간 시간 측정 결과
    TCNT1 = 0;     // 타이머 초기화 (다음 측정을 위해)
}

/*
 * main 함수
 * - Timer0: 외부 펄스를 만들어서 입력 캡처 테스트용 (PB4에서 발생)
 * - Timer1: 외부 입력(PD6)의 상승에지 감지 → ICR1에 시간 저장
 * - 세그먼트에 측정된 시간 값(m_cnt) 표시
 */
int main(void) {
    // 포트 설정
    DDRA = 0xFF;   // PORTA: 7-Segment 출력용 (a~g)
    DDRC = 0x0F;   // PORTC: 자릿수 선택용 (4자리)
    PORTA = 0xFF;  // 초기화 (모든 세그먼트 OFF)
    PORTC = 0x0F;  // 초기화 (모든 자릿수 OFF)

    // 타이머0 설정 (CTC 모드, 비교 매치 시 토글 출력)
    // -> PB4(OC0)에서 50% 듀티의 사각파 발생
    DDRB = 0x10;         // PB4 (OC0) 출력 설정
    TCCR0 = 0x1E;        // CTC 모드, 분주비 256, Compare Match 시 토글
    OCR0 = 150;          // TOP 값 (주기 조절)
    TCNT0 = 0;           // 초기값

    // Timer1 입력 캡처 인터럽트 활성화
    TIMSK = (1 << TICIE1);  // 입력 캡처 인터럽트 활성화

    // Timer1 설정
    // ICNC1=0(노이즈 제거 없음), ICES1=1(상승에지 감지), CS12=1 → 분주비 256
    // 클럭 = 16MHz / 256 = 62.5kHz → 1 tick ≈ 16μs
    TCCR1B = 0x44;

    sei();  // 전역 인터럽트 허용

    // 메인 루프
    while (1) {
        Segment(m_cnt);  // 측정된 시간 간격 값을 7세그먼트에 출력
    }
}
