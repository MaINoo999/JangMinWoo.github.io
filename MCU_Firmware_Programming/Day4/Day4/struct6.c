//#include <stdio.h>  // 표준 입출력 함수 사용
//
//// 구조체 정의
//// 자기 자신을 가리키는 포인터 멤버를 포함하는 "자기참조 구조체"
//struct node {
//	int num;                // 데이터 저장용 정수 변수
//	struct node* next;      // 다음 노드를 가리키는 포인터 (자기참조)
//};
//
//int main() {
//	// 구조체 변수 3개 생성 및 초기화
//	struct node n = { 10, 0 };   // 첫 번째 노드: 값은 10, 다음 노드는 아직 없음
//	struct node n1 = { 20, 0 };  // 두 번째 노드: 값은 20
//	struct node n2 = { 30, 0 };  // 세 번째 노드: 값은 30
//
//	// 포인터 head 선언, 첫 번째 노드를 가리킴
//	struct node* head = &n;
//
//	// 연결 리스트 구성
//	n.next = &n1;   // 첫 번째 노드의 next가 두 번째 노드를 가리키게 설정
//	n1.next = &n2;  // 두 번째 노드의 next가 세 번째 노드를 가리키게 설정
//
//	// 연결된 노드의 값 출력
//	printf("head->node: %d\n", head->num);                      // 첫 번째 노드의 값 출력 (10)
//	printf("head->next->node: %d\n", head->next->num);          // 두 번째 노드의 값 출력 (20)
//	printf("head->next->node->num: %d\n", head->next->next->num); // 세 번째 노드의 값 출력 (30)
//
//	return 0;  // 프로그램 정상 종료
//}
