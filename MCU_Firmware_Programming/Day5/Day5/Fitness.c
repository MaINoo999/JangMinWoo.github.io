#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 20

// 회원 정보를 저장하는 구조체 정의
typedef struct {
    int num;                // 회원 번호
    char name[MAX_NAME_LEN]; // 회원 이름
    double weight;          // 회원 체중
} Fitness;

// 전체 등록 회원 수 출력 함수
void total_number(int count) {
    printf("\n전체 등록 회원 수: %d명\n", count);
}

// 평균 체중을 계산하는 함수
double average_weight(Fitness** pary, int count) {
    double sum = 0.0;

    for (int i = 0; i < count; i++) {
        sum += pary[i]->weight;
    }

    return (count > 0) ? (sum / count) : 0.0;
}

// 최고 체중을 가진 회원의 인덱스를 반환하는 함수
int max_weight(Fitness** pary, int count) {
    if (count == 0) return -1;

    int maxIndex = 0;
    double maxWeight = pary[0]->weight;

    for (int i = 1; i < count; i++) {
        if (pary[i]->weight > maxWeight) {
            maxWeight = pary[i]->weight;
            maxIndex = i;
        }
    }

    return maxIndex;
}

// 특정 인덱스의 회원 정보 출력 함수
void print_info(Fitness** pary, int index) {
    if (index < 0) {
        printf("해당 회원 정보가 없습니다.\n");
        return;
    }

    printf("\n--- 회원 정보 ---\n");
    printf("번호: %d\n", pary[index]->num);
    printf("이름: %s\n", pary[index]->name);
    printf("체중: %.2f kg\n", pary[index]->weight);
}

// 동적으로 할당된 메모리를 해제하는 함수
void free_ary(Fitness** pary, int count) {
    for (int i = 0; i < count; i++) {
        free(pary[i]);  // 각 회원 정보 메모리 해제
    }
    free(pary);         // 포인터 배열 자체 메모리 해제
}

int main() {
    int count;

    printf("등록할 회원 수를 입력하세요: ");
    scanf("%d", &count);

    // Fitness 구조체 포인터 배열을 동적으로 할당
    Fitness** members = (Fitness**)malloc(sizeof(Fitness*) * count);

    if (members == NULL) {
        printf("메모리 할당 실패\n");
        return 1;
    }

    // 회원 정보 입력
    for (int i = 0; i < count; i++) {
        members[i] = (Fitness*)malloc(sizeof(Fitness));
        if (members[i] == NULL) {
            printf("메모리 할당 실패\n");
            return 1;
        }

        printf("\n[%d번째 회원 정보 입력]\n", i + 1);
        printf("회원 번호: ");
        scanf("%d", &members[i]->num);

        printf("이름: ");
        scanf("%s", members[i]->name);

        printf("체중 (kg): ");
        scanf("%lf", &members[i]->weight);
    }

    // 전체 등록 회원 수 출력
    total_number(count);

    // 평균 체중 출력
    double avg = average_weight(members, count);
    printf("전체 회원의 평균 체중: %.2f kg\n", avg);

    // 최고 체중 회원 정보 출력
    int maxIndex = max_weight(members, count);
    printf("\n[최고 체중 회원 정보 출력]\n");
    print_info(members, maxIndex);

    // 메모리 해제
    free_ary(members, count);

    return 0;
}
