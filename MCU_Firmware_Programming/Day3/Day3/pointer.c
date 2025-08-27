//#include <stdio.h>
//
//void main() {
//	int a = 10;										// 변수선언
//	int* pa;										// 포인터 선언(int 안에서는 *포인터변수 기호)
//	pa = &a;										// 변수a의 주소값을 포인터변수에 대입
//
//	printf("&a: %p, pa: %p\n", &a, pa);
//	//printf("&a: %u, pa: %u\n", &a, pa);	
//	printf("a: %d, *pa: %d\n", a, *pa);				// *(코드 안에서는 포인터 연산자가 작동한다)
//
//	*pa = 20;
//	printf("a: %d, *pa: %d\n", a, *pa);
//	printf("pa + 1: %p\n", pa + 1);					//포인터의 크기는 4바이트 크기를 가지고있어 4바이트씩 증가한다.
//	char c = 'a';
//	pa = &c;
//	printf("pa: %p, pa + 1: %p\n", pa, pa + 1);
//}