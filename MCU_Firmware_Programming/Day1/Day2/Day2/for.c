/*for(초기식, 조건식, 증감식) { }*/
#include <stdio.h>

void main() {
	for (int i = 0; i < 10; i++) {						//i=0 초기식 , i<10 조건식 , i++ 증감식
		printf("%d ", i+1);
		printf("%d ", i);
	}
}