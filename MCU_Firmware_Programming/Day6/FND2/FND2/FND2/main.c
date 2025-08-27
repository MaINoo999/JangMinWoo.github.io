/*
 * FND_Display.c
 *
 * Created: 2025-08-12
 * Author : COMPUTER
 * 
 * 목적:
 * - 7세그먼트 FND(공통 캐소드)에 숫자 0~9를 순차적으로 출력
 * - PORTB: 세그먼트 제어 (a~g, dp)
 * - PORTG: 자릿수 제어 (여러 자리 FND 사용 시)
 * 
 * 주의:
 * - 공통 캐소드(Common Cathode) 기준: HIGH(1) → 세그먼트 ON
 */

#include <avr/io.h>       // AVR 입출력 레지스터 정의
#include <util/delay.h>   // _delay_ms() 함수 사용

// 7세그먼트 숫자 및 알파벳 출력용 패턴 배열 (공통 캐소드용)
// 각 비트는 a~g, dp에 해당
// 예: 0b0111111 → a~f 켜짐 (0 표시)
unsigned char Font[16] = {
    0b00111111, // 0: a b c d e f
    0b00000110, // 1: b c
    0b01011011, // 2: a b d e g
    0b01001111, // 3: a b c d g
    0b01100110, // 4: b c f g
    0b01101101, // 5: a c d f g
    0b01111101, // 6: a c d e f g
    0b00000111, // 7: a b c
    0b01111111, // 8: a b c d e f g
    0b01101111, // 9: a b c d f g
    0b01110111, // A
    0b01111100, // b
    0b00111001, // C
    0b01011110, // d
    0b01111001, // E
    0b01110001  // F
};

// 숫자 1234 나타내기, 자릿수 선택 (PG0~PG3: LOW가 활성화)
unsigned char digitSelect[4] = {
	0b1110, // 1(일의 자리)
	0b1101, // 2(십의 자리)
	0b1011, // 3(백의 자리)
	0b0111  // 4(천의 자리)
};

int main(void)
{
    // 포트 설정
    DDRB = 0xFF;   // PORTB (PB0~PB7)를 출력으로 설정 → 세그먼트 제어
    DDRG = 0xFF;   // PORTG (PG0~PG7)를 출력으로 설정 → 자릿수 선택 제어
    PORTG = 0x00;  // 첫 번째 자릿수 선택 (PG0만 LOW → 해당 자릿수 FND 활성화)
/*    while (1) // 무한 반복
    {
        for (int i = 0; i <= 15; i++)  // 숫자 0~9,A~F 출력 반복
        {
            PORTB = Font[i];  // 현재 숫자 i에 해당하는 세그먼트 패턴을 PORTB에 출력
                             // → 해당 숫자가 FND에 표시됨

            _delay_ms(10000);  // 10초 동안 유지 10000ms 딜레이)
        }
    }
}
*/
while (1)			//  숫자 1234 나타내기
{
	// 표시할 숫자 1234 → 각 자리 분리
	unsigned char numbers[4] = {1, 2, 3, 4};  // 일~천의 자리 순

	// 1프레임 표시 (짧은 시간 동안 각 자리 반복)
	for (int i = 0; i < 4; i++)
	{
		PORTB = Font[numbers[i]];     // 해당 자리 숫자 표시
		PORTG = digitSelect[i];       // 해당 자리 선택 (PGn = 0)
		_delay_ms(2);                 // 잠깐 표시 (잔상 효과)
	}
}
}