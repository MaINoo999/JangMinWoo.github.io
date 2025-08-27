//#include <stdio.h> 
// 표준 입출력 라이브러리. printf 함수를 사용하기 위해 필요합니다.


// 매크로 정의: 두 토큰 x와 y를 붙여 하나의 이름으로 만듦.
// 예: NAME_CAT(a, 1) → a1
//#define NAME_CAT(x, y)   (x ## y)


// 매크로 정의: 매개변수 x를 문자열로 바꾸고, 해당 변수의 값을 출력하는 코드 생성
// 예: PRINT_EXPR(a1 + a2) → printf("a1 + a2 = %d\n", a1 + a2)
//#define PRINT_EXPR(x)  printf(#x " = %d\n", x)


//int main() {
    // 정수형 변수 a1, a2 선언
//	int a1, a2;

    // NAME_CAT(a, 1) → a1 이므로, a1 = 10; 이 되는 것과 같음
//	NAME_CAT(a, 1) = 10;

    // NAME_CAT(a, 2) → a2 이므로, a2 = 20; 이 되는 것과 같음
//	NAME_CAT(a, 2) = 20;

    // PRINT_EXPR(a1 + a2) → printf("a1 + a2 = %d\n", a1 + a2);
//	PRINT_EXPR(a1 + a2);

    // 위 매크로를 사용하지 않고 직접 작성한 출력문
    //printf("a1+a2 = %d\n", a1 + a2);

//	return 0;
//}
