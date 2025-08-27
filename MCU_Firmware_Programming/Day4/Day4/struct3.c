//#include <stdio.h> // 표준 입출력 함수 사용을 위한 헤더 파일 포함
//
//// vision 구조체 정의
//// 왼쪽 시력(left)과 오른쪽 시력(right)을 저장
//struct vision {
//	double left;   // 왼쪽 시력
//	double right;  // 오른쪽 시력
//};
//
//// 함수 선언 (교환 함수)
//// 매개변수로 받은 시력 정보를 교환한 후 구조체로 반환
//struct vision exchange(struct vision arobot);
//
//// 메인 함수 시작
//void main() {
//	struct vision robot;  // vision 구조체 변수 선언
//
//	// 사용자로부터 왼쪽과 오른쪽 시력 입력받기
//	printf("시력 입력: ");
//	scanf_s("%lf %lf", &robot.left, &robot.right);
//	// scanf_s는 보안 강화된 입력 함수로 Visual Studio에서 사용
//
//	// 시력 정보를 교환하는 함수 호출
//	robot = exchange(robot);
//
//	// 교환된 시력 정보 출력
//	printf("바뀐 시력: %.1lf   %.1lf\n", robot.left, robot.right);
//}
//
//// 시력 정보를 교환하는 함수 정의
//struct vision exchange(struct vision arobot)
//{
//	double temp;  // 임시 변수 선언
//
//	// 왼쪽과 오른쪽 시력 값 교환
//	temp = arobot.left;          // 왼쪽 시력을 temp에 저장
//	arobot.left = arobot.right;  // 오른쪽 시력을 왼쪽에 저장
//	arobot.right = temp;         // temp에 있던 왼쪽 시력을 오른쪽에 저장
//
//	return arobot; // 교환된 구조체 반환
//}
