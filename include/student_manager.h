#pragma once

/* 学生信息结构体：学号 + 姓名 + 三科成绩 */
typedef struct Student
{
	unsigned long long number;   /* 学号，位数较多，用 64 位无符号整型 */
	char name[32];               /* 姓名，最多 31 个字节 */
	float chinese;               /* 语文成绩 */
	float math;                  /* 数学成绩 */
	float english;               /* 英语成绩 */
} Student;

/* 单链表节点 */
typedef struct Node
{
	Student stu;                 /* 数据域：一名学生的完整信息 */
	struct Node* next;           /* 指针域：指向下一个节点 */
} Node;

/* 链表表头：只保存首节点指针和人数，不为数据额外分配内存 */
typedef struct List
{
	Node* front;                 /* 首节点（头插法插入，最新录入的在最前面） */
	int size;                    /* 当前链表中的学生人数 */
} List;

/* 菜单项：数值与 menu() 返回的序号、switch 的分支一一对应 */
typedef enum MenuOptions
{
	Quit,        /* 0 退出系统 */
	Entry,       /* 1 录入学生信息 */
	Print,       /* 2 打印学生信息 */
	Save,        /* 3 保存学生信息 */
	Read,        /* 4 读取学生信息 */
	Statistics,  /* 5 统计学生信息 */
	Find,        /* 6 查找学生信息 */
	Alter,       /* 7 修改学生信息 */
	Remove,      /* 8 删除学生信息 */
} MenuOptions;

/* 打印主菜单，返回用户选择的功能序号 */
int menu();

/* 头插法录入一名学生 */
void entryStudent(List* list);

/* 遍历链表，按表格打印全部学生 */
void printStudent(List* list);

/* 以二进制形式保存到 students.data（机器可读，体积小） */
void saveStudent(List* list);
/* 以文本形式保存到 student.txt（人可读，可手动编辑） */
void saveStudentHuman(List* list);

/* 从 students.data 读取（二进制） */
void readStudent(List* list);
/* 从 student.txt 读取（文本） */
void readStudentHuman(List* list);

/* 统计学生人数 */
void statisticsStudentCount(List* list);

/* 按学号或姓名查找，返回命中的节点，找不到返回 NULL */
Node* findStudent(List* list);

/* 按学号或姓名定位学生并修改其三科成绩 */
void alterStudent(List* list);

/* 按学号或姓名删除学生节点 */
void removeStudent(List* list);
