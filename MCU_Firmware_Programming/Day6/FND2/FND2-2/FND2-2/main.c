 /*
 * FND2-2.c
 *
 * Created: 2025-08-12 오후 2:15:28
 * Author : COMPUTER
 * Description: 버튼 8개를 누르면 각각 대응되는 LED가 켜지고,
 *             손을 떼면 해당 LED가 꺼지는 코드
 */ 

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRE = 0xFF;    // PORTE: LED용, 출력
    DDRD = 0x00;    // PORTD: 버튼용, 입력
    PORTD = 0xFF;   // 내부 풀업 저항 활성화
    PORTE = 0x00;   // LED OFF로 초기화

    while (1) 
    {
        /*
         * 해당 값을 PORTE에 출력하면 버튼에 대응하는 LED가 켜짐.
         * 버튼을 놓으면 다시 LED가 꺼짐.
         */
        PORTE = PIND;
    }
}
