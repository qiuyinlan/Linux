#include <stdio.h>
#include <string.h>
#define MAX_STUDENTS 100

// 学生结构体
typedef struct {
    char name[50];
    int score;
} Student;

// 学生数组
Student students[MAX_STUDENTS];
// 当前数量
int numStudents = 0;

// 函数声明
void inputRecord();
void deleteRecord();
void findRecord();
void sortRecord();
void insertRecord();
void displayRecord();

int main() {
    int choice;
    do {
        printf("1.成绩录入\n");
        printf("2.成绩删除\n");
        printf("3.成绩查找\n");
        printf("4.成绩排序\n");
        printf("5.成绩插入\n");
        printf("6.退出成绩系统\n");
        printf("请输入操作: ");
        scanf("%d", &choice);

        switch(choice) {
            case 6:
                printf("程序退出\n");
                break;
            case 1:
                inputRecord();
                break;
            case 2:
                // deleteRecord();
                break;
            case 3:
                // findRecord();
                break;
            case 4:
                // sortRecord();
                break;
            case 5:
                // insertRecord();
                break;
            default:
                printf("无效选项，请重新选择。\n");
        }
    } while(choice != 6);

    return 0;
}

void inputRecord() {
    if (numStudents == MAX_STUDENTS) {
        printf("学生数量最大, 无法添加记录\n");
        return;
    }

    Student newStudent;
    printf("请输入姓名: ");
    scanf("%s", newStudent.name);
    printf("请输入成绩: ");
    scanf("%d", &newStudent.score);

    students[numStudents] = newStudent;
    numStudents++;
    printf("学生成绩记录添加成功\n");
}

void displayRecord() {
    printf("姓名：成绩\n");
    for (int i = 0; i < numStudents; i++) {
        printf("%s, %d\n", students[i].name, students[i].score);
    }
}
