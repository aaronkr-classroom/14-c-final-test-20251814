#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* =====================
   Student 구조체
   ===================== */
typedef struct {
    char name[20];
    int kor;
    int eng;
    int math;
    int total;
    float avg;
    int rank;
} Student;

/* =====================
   연결 리스트 Node
   ===================== */
typedef struct Node {
    Student data;
    struct Node* p_next;
} Node;

/* =====================
   학생 노드 추가
   ===================== */
void AddStudent(Node** pp_head, Node** pp_tail, Student s) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("메모리 할당 실패\n");
        return;
    }

    newNode->data = s;
    newNode->p_next = NULL;

    if (*pp_head == NULL) {
        *pp_head = newNode;
        *pp_tail = newNode;
    }
    else {
        (*pp_tail)->p_next = newNode;
        *pp_tail = newNode;
    }
}

/* =====================
   총점 / 평균 계산
   ===================== */
void Calculate(Node* head) {
    Node* p = head;
    while (p != NULL) {
        p->data.total = p->data.kor + p->data.eng + p->data.math;
        p->data.avg = p->data.total / 3.0f;
        p = p->p_next;
    }
}

/* =====================
   등수 계산
   ===================== */
void CalculateRank(Node* head) {
    Node* p, * q;

    for (p = head; p != NULL; p = p->p_next) {
        p->data.rank = 1;
        for (q = head; q != NULL; q = q->p_next) {
            if (p->data.total < q->data.total)
                p->data.rank++;
        }
    }
}

/* =====================
   성적 출력
   ===================== */
void PrintStudents(Node* head) {
    Node* p = head;

    if (p == NULL) {
        printf("출력할 데이터가 없습니다.\n");
        return;
    }

    printf("\n이름\t국어\t영어\t수학\t총점\t평균\t등수\n");
    printf("--------------------------------------------------\n");

    while (p != NULL) {
        printf("%s\t%d\t%d\t%d\t%d\t%.2f\t%d\n",
            p->data.name,
            p->data.kor,
            p->data.eng,
            p->data.math,
            p->data.total,
            p->data.avg,
            p->data.rank);
        p = p->p_next;
    }
}

/* =====================
   학생 정보 입력
   ===================== */
void InputStudent(Node** pp_head, Node** pp_tail) {
    Student s;

    printf("이름: ");
    scanf("%19s", s.name);
    printf("국어 점수: ");
    scanf("%d", &s.kor);
    printf("영어 점수: ");
    scanf("%d", &s.eng);
    printf("수학 점수: ");
    scanf("%d", &s.math);

    s.total = 0;
    s.avg = 0.0f;
    s.rank = 0;

    AddStudent(pp_head, pp_tail, s);
}

/* =====================
   파일에서 읽기 (.dat)
   ===================== */
void LoadFromFile(Node** pp_head, Node** pp_tail) {
    FILE* fp = fopen("students.dat", "rb");
    Student s;

    if (fp == NULL) {
        printf("students.dat 파일이 없습니다.\n");
        return;
    }

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        AddStudent(pp_head, pp_tail, s);
    }

    fclose(fp);
    printf("파일에서 데이터 읽기 완료\n");
}

/* =====================
   파일에 저장 (.dat)
   ===================== */
void SaveToFile(Node* head) {
    FILE* fp = fopen("students.dat", "wb");
    Node* p = head;

    if (fp == NULL) {
        printf("파일 저장 실패\n");
        return;
    }

    while (p != NULL) {
        fwrite(&p->data, sizeof(Student), 1, fp);
        p = p->p_next;
    }

    fclose(fp);
    printf("파일 저장 완료\n");
}

/* =====================
   메모리 해제
   ===================== */
void FreeList(Node* head) {
    Node* p;
    while (head != NULL) {
        p = head;
        head = head->p_next;
        free(p);
    }
}

/* =====================
   main 함수
   ===================== */
int main(void) {
    Node* p_head = NULL;
    Node* p_tail = NULL;
    int menu;

    while (1) {
        printf("\n[Menu]\n");
        printf("1. .dat 파일에서 데이터 읽기\n");
        printf("2. 추가 학생 정보 입력\n");
        printf("3. .dat 파일 저장\n");
        printf("4. 성적 확인\n");
        printf("5. 종료\n");
        printf("-------------------\n");
        printf("선택(1~5): ");
        scanf("%d", &menu);

        switch (menu) {
        case 1:
            LoadFromFile(&p_head, &p_tail);
            break;

        case 2:
            InputStudent(&p_head, &p_tail);
            break;

        case 3:
            SaveToFile(p_head);
            break;

        case 4:
            Calculate(p_head);
            CalculateRank(p_head);
            PrintStudents(p_head);
            break;

        case 5:
            FreeList(p_head);
            printf("프로그램 종료\n");
            return 0;

        default:
            printf("잘못된 선택입니다.\n");
        }
    }
}
