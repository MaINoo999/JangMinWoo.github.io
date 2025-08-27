#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>

// USART 초기화 함수 (보드레이트 115200bps, 8N1)
void USART_init(void)
{
	// 송신 및 수신 활성화 (TX, RX enable)
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	// 데이터 비트: 8비트, 패리티 없음, 1스톱 비트
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

	// 보드레이트 설정: 14.7456MHz에서 115200bps (16배속 모드 기준)
	UBRR0H = 0;
	UBRR0L = 7;
}

// 문자 1바이트 송신
void USART_tx(char data)
{
	while (!(UCSR0A & (1 << UDRE0))); // 송신 가능 상태 대기
	UDR0 = data;
}

// 문자 1바이트 수신
char USART_rx(void)
{
	while (!(UCSR0A & (1 << RXC0))); // 수신 데이터 도착 대기
	return UDR0;
}

// 문자열 송신
void USART_print(const char* str)
{
	while (*str) {
		USART_tx(*str++);
	}
}

int main(void)
{
	USART_init();        // USART 초기화
	USART_print("Hello world!\r\n"); // 초기 메시지 출력

	while (1)
	{
		char data = USART_rx();  // 수신된 문자 읽기
		USART_tx(data);          // 받은 문자 다시 송신 (에코)
		_delay_ms(1000);         // 1초 지연
	}
}
