//#include <stdio.h>  // 표준 입출력 헤더 포함
//
//// profile 구조체 정의
//// 학생의 나이와 이름 정보를 저장
//struct profile {
//	int age;           // 나이
//	char name[20];     // 이름 (문자열, 최대 19자 + null 문자)
//};
//
//// student 구조체 정의
//// profile 구조체를 멤버로 포함하여 중첩 구조체로 구성
//struct student {
//	struct profile p;  // 중첩된 구조체: 학생의 프로필 정보
//	int id;            // 학번
//	double grade;      // 성적
//};
//
//void main() {
//	// profile 구조체 변수 선언 및 초기화
//	struct profile p = { 100, "홍길동" };
//
//	// 이 시점에서는 student 구조체를 사용하진 않았지만,
//	// 위에서 정의한 중첩 구조체 예제를 위한 준비 단계로 볼 수 있음
//
//	// 출력 예시 (테스트용)
//	printf("이름: %s\n", p.name);
//	printf("나이: %d\n", p.age);
//}
