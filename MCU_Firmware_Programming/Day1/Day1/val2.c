/*
#include <stdio.h>

int main()
{
	char ch = 0x7f;						//0b 0111 1111
	unsigned ch1 = 0x7f;
	
	char ch2 = 0x9f;					//0b 1001 1111
	unsigned char ch3 = 0x9f;

	char ch4 = 0x11f;					//ob 1 0001 1111
	unsigned char ch5 = 0x11f;



	printf("ch:  %#x\t  ch1: %#x\n", ch, ch1);
	printf("ch2: %#x\t  ch3: %#x\n", ch2,ch3);
	printf("ch4: %#x\t  ch5: %#x\n", ch4, ch5);

	//ch = ch << 1;						// ob 1001 1111 => ob 0011 1110
	//ch2 = ch2 << 1;
	//printf("ch2: %#x\t ch2: %#x\n", ch, ch2);

	ch = ch >> 1;						// ob 1001 1111 => ob ffffffcf
	ch2 = ch2 >> 1;						// 0b 1001 1111 => 0b 0100 1111
	printf("ch: %#x\t ch3: %#x\n", ch, ch2);

		return 0;
}
*/