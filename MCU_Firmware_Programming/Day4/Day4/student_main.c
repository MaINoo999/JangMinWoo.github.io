//#include <stdio.h> 
// 표준 입출력 함수 사용을 위한 헤더 파일. printf, scanf_s 등을 사용하기 위해 필요함.

//#include "student.h"
// 사용자 정의 헤더 파일 포함. 해당 파일에는 구조체 Student 정의가 포함되어 있어야 함.
// 예: typedef struct { int id; char name[20]; } Student;


//#define PI 3,14
// 문제점 있는 매크로 정의! 
// 3,14 → 컴파일러는 "3"과 "14"를 쉼표 연산자로 인식하여, **14만 남게 됨**.
// 즉, area = radius * radius * PI → 실제 계산은 radius * radius * 14가 됨!
// 올바른 정의는: #define PI 3.14


//int main() {
//
//	double radius, area; // 반지름과 넓이를 저장할 변수 선언
//
//	Student a = { 900, "홍길동" };
//	// 구조체 Student 타입 변수 a 선언 및 초기화
//	// 900은 학번(id), "홍길동"은 이름(name)
//
//	printf("name: %s, 학번: %d\n", a.name, a.id);
//	// 구조체 멤버에 접근하여 이름과 학번 출력
//
//	printf("반지름: ");
//	scanf_s("%lf", &radius);
//	// scanf_s는 보안 기능이 있는 입력 함수 (Visual Studio 등에서 권장 사용)
//	// %lf는 double형 입력을 받기 위한 서식 지정자
//
//	area = radius * radius * PI;
//	// 매크로 PI가 잘못 정의되어 있어 잘못된 결과가 나올 수 있음!
//	// 제대로 계산하려면 PI를 3.14로 수정해야 함
//
//	printf("원의 넓이: %.1lf\n", area);
//	// 넓이 출력. 소수점 아래 첫째 자리까지 출력
//
//	return 0;
//}
