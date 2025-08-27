///* 3명의 4과목 점수를 입력받아 총점과 평균을 구하시오. */
//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//
//void main() {
//	
//	int score[3][4];					//2차원 배열
//	double avg;
//	int total;
//
//        // 점수 입력
//    for (int i = 0; i < 3; i++) {
//        printf("%d번째 학생의 4과목를 입력하세요.:\n", i + 1);
//        for (int j = 0; j < 4; j++) {
//            printf("%d번째 과목 점수: ", j + 1);
//            scanf("%d", &score[i][j]);
//        }
//    }
//
//    printf("\n--- 학생별 총점과 평균 ---\n");
//
//    // 총점과 평균 계산 및 출력
//    for (int i = 0; i < 3; i++) {
//        total = 0;
//        for (int j = 0; j < 4; j++) {
//            total += score[i][j];
//        }
//        avg = total / 4.0;
//
//        printf("%d번째 학생 - 총점: %d, 평균: %.2f\n", i + 1, total, avg);
//    }
//}