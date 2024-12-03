#include <stdio.h>
#include <string.h>
#define  MAX_STUDENTS  100


//学生结构体
typedef struct
{
	char name[50];
	int score;
}Student;

//学生数组
Student student[MAX_STUDENTS];
//当前数量
int numStudents=0;
//函数声明
//1.成绩录入
void inputRecord();
//2.成绩删除
void deleteRecord();
//3.成绩查找
void findRecord();
//4.成绩排序
void sortRecord();
//5.成绩插入
void insertRecord();
//6.退出成绩系统
void 

int main(){
	int choice;
	do {
		printf("1.成绩录入\n");
    	printf("2.成绩删除\n");
        printf("3.成绩查找\n");
        printf("4.成绩排序\n");
        printf("5.成绩插入\n");
        printf("6.退出成绩系统\n");
		printf("请输入操作:");
		scanf("%d",&choice);

		switch(choice) {
			case 0:
				printf("程序退出\n");
				break;
			case 1:
                 inputRecord();
				break;
		}
	} while(choice!=0);
    return 0;

}

void inputRecord() {
	//判断学生数量是否最大
	if(numStudents==MAX_STUDENTS) {
		printf("学生数量最大,无法添加记录");
		return;
	}

	//输入姓名和成绩
	Student student;
	printf("请输入姓名:");
	scanf("%s,&student.name);
	printf("请输入成绩:");
	scanf("%d",&student.score);
	student[numStudents]=student;

	numStudents++;
	printf("学生成绩记录添加成功\n");
}

void displayRecord() {
	printf("姓名：成绩\n");
	for(int i=0;i<=numStudents-1;i++) {
		Student student = student[i];
		printf("%s,%d\n",student.name, student.score);
	}

}