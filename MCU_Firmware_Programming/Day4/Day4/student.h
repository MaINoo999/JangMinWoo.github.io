//#pragma once
// 이 지시문은 헤더 파일이 **한 번만 포함되도록** 보장함.
// 동일한 헤더 파일이 여러 번 포함되어도 컴파일러가 중복 포함을 방지해줌.
// 전통적인 include guard (#ifndef ~ #define ~ #endif)보다 간단하고 편리함 (비표준이지만 대부분 컴파일러에서 지원됨)


//typedef struct {
//	int id;            // 학번 또는 ID를 저장하는 정수형 변수
//	char name[20];     // 이름을 저장하는 문자열 배열 (최대 19자 + null 문자 '\0')
//} Student;
// 구조체 정의와 동시에 typedef로 별칭(Student)을 정의하여
// 구조체를 선언할 때 'struct' 키워드 없이도 사용할 수 있도록 함.
//
// 예시: Student a; ← struct Student a; 와 동일한 의미