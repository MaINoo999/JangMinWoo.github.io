//#include <stdio.h>												// 표준 입출력 라이브러리 포함
//
//	// address 구조체 정의
//struct address {
//	char name[20];											    // 이름 (최대 19자 + NULL)
//	int age;													// 나이 (여기서는 연도처럼 사용됨)
//	char tel[50];												// 전화번호
//	char addr[80];												// 주소
//};
//
//// 주소록 리스트 출력 함수 정의
//// 구조체 배열을 받아서 각 항목의 정보를 출력
//void print_list(struct address list[]) {
//	for (int i = 0; i < 3; i++) {								// 총 3명의 정보 출력
//		// 이름, 나이, 전화번호, 주소 순서로 출력
//		printf("%s %5d %15s %20s\n",
//			list[i].name,   // 이름 출력
//			list[i].age,    // 나이 출력 
//			list[i].tel,    // 전화번호 출력 
//			list[i].addr    // 주소 출력 
//		);
//	}
//}
//
//int main() {
//	// address 구조체 배열 선언 및 초기화 (3명 정보 저장)
//	struct address list[3] = {
//		{ "홍길동", 900, "010 - 3322 - 3233", "서울 강남구" },
//		{ "김날길", 800, "010 - 3322 - 3233", "대전 서구" },
//		{ "구나딩", 700, "010 - 3322 - 3233", "대구 동구" }
//	};
//
//	// 리스트 출력 함수 호출
//	print_list(list);
//
//	return 0; // 프로그램 종료
//}