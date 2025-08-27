//#include <stdio.h>  // 표준 입출력 함수들을 사용하기 위한 헤더 파일
//
//// 계절을 나타내는 열거형(enum) 정의
//enum season {
//    SPRING, // 0
//    SUMMER, // 1
//    FALL,   // 2
//    WINTER  // 3
//};
//
//int main() {
//    enum season s;    // 열거형 변수 s 선언 (계절을 저장할 변수)
//    char* ps = NULL;  // 문자열 포인터 ps 선언 및 초기화 (레저 활동 이름 저장용)
//
//    s = SUMMER;  // 현재 계절을 SUMMER(여름)로 설정
//
//    // s의 값에 따라 적절한 문자열을 ps에 할당하는 switch문
//    switch (s) {
//    case SPRING:  // s가 SPRING(0)이면
//        ps = "Inline skating";  // 봄 레저활동 설정
//        break;                 // switch 종료
//    case SUMMER:  // s가 SUMMER(1)이면
//        ps = "Swimming";       // 여름 레저활동 설정
//        break;                 // switch 종료
//    case FALL:    // s가 FALL(2)이면
//        ps = "Hiking";         // 가을 레저활동 설정
//        break;                 // switch 종료
//    case WINTER:  // s가 WINTER(3)이면
//        ps = "Skiing";         // 겨울 레저활동 설정
//        break;                 // switch 종료
//    default:      // 예기치 않은 값이 들어왔을 때 대비용
//        ps = "No activity";   // 기본 문자열 설정
//        break;
//    }
//
//    // 결과 출력, 한글 "계절별 레저활동: "과 문자열 ps를 함께 출력
//    printf("계절별 레저활동: %s\n", ps);
//
//    return 0;  // 프로그램 정상 종료
//}
