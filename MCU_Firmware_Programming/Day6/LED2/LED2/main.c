/*
 * LED2.c
 *
 * Created: 2025-08-11 오후 3:48:16
 * Author : COMPUTER
쉬프트
 */ 

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	/* Replace with your application code */
    unsigned char LED = 0x01;
	DDRE = 0xff;					// PORT를 출력 핀으로 설정
	
	while (1) 
    {
		PORTE = LED;
		_delay_ms(500);
		LED = LED << 1;
		/*
		PORTE = 0x01;
		_delay_ms(500);
		PORTE = 0x02;
		_delay_ms(500);
		PORTE = 0x04;
		_delay_ms(500);
		PORTE = 0x10;
		_delay_ms(500);
		PORTE = 0x20;
		_delay_ms(500);
		PORTE = 0x40;
		_delay_ms(500);
		PORTE = 0x80;
		_delay_ms(500);*/
    }
}

