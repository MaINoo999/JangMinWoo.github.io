//#include <stdio.h>
//void swap(int, int);										//함수 원형을 적을떄는 매개변수는 안적어도 된다.
//void swap1(int* pa, int* pb);
//
//void main() {
//	int a = 10, b = 20;
//	printf("a: %d, b: %d\n", a, b);
//	/* 값에의한 전달*/
//	swap(a, b);
//	/* 주소에 의한 전달 */
//	swap1(&a, &b);											//&변수의 주소값 
//	printf("a: %d, b: %d\n", a, b);
//}
//void swap(int x, int y)
//{
//	int temp = x;
//	x = y;
//	y = temp;
//	printf("x: %d, y: %d\n", x, y);
//}
//void swap1(int* pa,int* pb)
//{
//	int temp = *pa;
//	*pa = *pb;
//	*pb = temp;
//}