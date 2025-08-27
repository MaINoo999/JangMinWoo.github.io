/*
 * FND.c
 *
 * Created: 2025-08-12 오전 10:05:42
 * Author : COMPUTER
 * 
 * FND 제어 방식 설명:
 * - 애노드 방식 (common anode): 공통 VCC, 각 단자는 GND로 연결되어 점등
 * - 캐소드 방식 (common cathode): 공통 GND, 각 단자는 VCC로 연결되어 점등
 */

#include <avr/io.h>      // AVR의 입출력 레지스터 정의 헤더
#include <util/delay.h>  // _delay_ms() 함수 사용을 위한 헤더

int main(void)
{
    DDRB = 0xff;    // 포트 B의 모든 핀을 출력으로 설정 (8비트 전부 출력)
    DDRG = 0xff;    // 포트 G의 모든 핀도 출력으로 설정
    PORTG = 0x07;   // 포트 G의 출력을 모두 0으로 초기화 (예: 선택된 FND가 없도록 설정) ob0000 0111

    while (1) 
    {
        // 포트 B에 0x01 출력 → 0000 0001 (가장 오른쪽 비트만 HIGH)
        // 이후 0.5초 대기
        PORTB = 0x01;
        _delay_ms(500);

        // 포트 B에 0x02 출력 → 0000 0010 (두 번째 비트만 HIGH)
        PORTB = 0x02;
        _delay_ms(500);

        // 포트 B에 0x04 출력 → 0000 0100 (세 번째 비트만 HIGH)
        PORTB = 0x04;
        _delay_ms(500);

        // 포트 B에 0x08 출력 → 0000 1000 (네 번째 비트만 HIGH)
        PORTB = 0x08;
        _delay_ms(500);

        // 포트 B에 0x10 출력 → 0001 0000 (다섯 번째 비트만 HIGH)
        PORTB = 0x10;
        _delay_ms(500);

        // 포트 B에 0x20 출력 → 0010 0000 (여섯 번째 비트만 HIGH)
        PORTB = 0x20;
        _delay_ms(500);

        // 포트 B에 0x40 출력 → 0100 0000 (일곱 번째 비트만 HIGH)
        PORTB = 0x40;
        _delay_ms(500);

        // 포트 B에 0x80 출력 → 1000 0000 (가장 왼쪽 비트만 HIGH)
        PORTB = 0x80;
        _delay_ms(500);
    }
}
