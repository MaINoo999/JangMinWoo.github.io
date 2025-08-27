//#define _CRT_SECURE_NO_WARNINGS  // Visual Studio에서 보안 경고 제거
//#include <stdio.h>               // 표준 입출력 함수
//#include <string.h>             // 문자열 관련 함수 사용 (e.g., strcpy)
//#include <stdlib.h>             // 동적 메모리 할당 함수 사용 (e.g., malloc)
//
//// 구조체 정의
//// 사람의 프로필 정보를 저장
//struct porfile {
//	char name[20];     // 이름 (문자열, 최대 19자 + '\0')
//	int age;           // 나이
//	double height;     // 키
//	double weight;     // 몸무게
//	char* info;        // 소개 문장 (동적 메모리로 저장 예정)
//};
//
//void main() {
//	// 구조체 변수 선언
//	struct porfile m;
//
//	// 정적 멤버들 초기화
//	m.age = 22;
//	m.height = 183;
//	m.weight = 80;
//
//	// 문자열 복사 (이름 초기화)
//	strcpy(m.name, "장민우");
//
//	// 동적 메모리 할당: 소개글 최대 80자 저장 가능
//	m.info = (char*)malloc(80);  // malloc은 void*를 반환하므로 형변환 필요
//
//	// 사용자로부터 소개글 입력 받기
//	printf("자기소개: ");
//	gets(m.info);  // 문자열을 공백 포함하여 입력 받음 (비추천 함수)
//
//	// 결과 출력
//	printf("\n--- 출력 결과 ---\n");
//	printf("이름: %s\n", m.name);
//	printf("나이: %d\n", m.age);
//	printf("키: %.1lf\n", m.height);
//	printf("몸무게: %.1lf\n", m.weight);
//	printf("소개: %s\n", m.info);
//
//	// 동적 메모리 해제
//	free(m.info);
//}
