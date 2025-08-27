///* 문자를 받아서 띄우는 코드 */
//#include <stdio.h>
//
//void main() {
//	char animal[5][20];															// 문자열 주소
//	int size;
//
//	size = sizeof(animal) / sizeof(animal[0]);
//	printf("%d\n", size);
//
//	for (int i = 0; i < size; i++) {
//		scanf_s("%s", animal[i], (unsigned)sizeof(animal[i]));					// animal[i] 캐릭터 배열의 1차원을 나타냄
//	}
//	for (int i = 0; i < size; i++) {
//		printf("%s ", animal[i]);
//	}
//}