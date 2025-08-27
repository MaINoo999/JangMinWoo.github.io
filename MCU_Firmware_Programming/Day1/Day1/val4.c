/*
	문자는 ' ', 문자열로 " " 로 묶는다.
	문자는 char, 문자열은 char 배열로 선언한다.
	배열명은 배열의 시작주소를 나타낸다.
	문자열은 문자열크기 + 1의 배열크기를 가진다 그래서 apple은 5글자지만 char str[6]에 숫자 6을 넣었다.
*/
/*
#include <stdio.h>

void main()
{
	char ch = 'a';							// 문자
	printf("ch: %c, ch: %d\n", ch, ch);

	char str[6] = "apple";					// 배열( 배열이름은 시작주소, for)
	printf("str: %s\n", str);

	printf("str[0]: %c\n", str[0]);
	printf("str[4]: %c\n", str[4]);
	printf("str[5]: %s\n", str[5]);			// 널문자 출력

	char str1[] = "banana";
	char str2[30] = str + str1;

	int a = 10, b = 20;
	int c = a + b;
}
*/