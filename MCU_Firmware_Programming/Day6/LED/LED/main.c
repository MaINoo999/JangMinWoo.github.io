/*
 * LED.c
 *
 * Created: 2025-08-11 오후 2:28:21
 * Author : COMPUTER
 */ 

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    /* Replace with your application code */
	DDRE = 0xff;								//DDRE(방향설정 레지스트: 1출력, 0 입력)= 1111 1111
    while (1) 
    {
		PORTE = 0xff;
		_delay_ms(500);
		PORTE = 0x00;
		_delay_ms(500);
		
    }
}

