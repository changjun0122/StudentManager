#include "student_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 申请一个节点并做基本初始化：data 由调用方填充，next 置空 */
static Node* CreateNode(void)
{
	Node* node = (Node*)malloc(sizeof(Node));
	if (!node)
	{
		printf("malloc failed\n");
		return NULL;
	}
	node->next = NULL;
	return node;
}

/* 打印主菜单，返回用户输入的序号 */
int menu(void)
{
	printf("**********************************\n");
	printf("*\t欢迎使用学生管理系统\t *\n");
	printf("**********************************\n");
	printf("*          请选择功能            *\n");
	printf("**********************************\n");
	printf("*        1.录入学生信息\t\t *\n");
	printf("*        2.打印学生信息\t\t *\n");
	printf("*        3.保存学生信息\t\t *\n");
	printf("*        4.读取学生信息\t\t *\n");
	printf("*        5.统计学生信息\t\t *\n");
	printf("*        6.查找学生信息\t\t *\n");
	printf("*        7.修改学生信息\t\t *\n");
	printf("*        8.删除学生信息\t\t *\n");
	printf("*        0.退出系统\t\t *\n");
	printf("**********************************\n");
	printf("select>");

	int select = -1;
	if (scanf("%d", &select) != 1)
	{
		if (feof(stdin))
		{
			/* 输入流已结束（如管道输入读完），直接当作退出，避免死循环 */
			return Quit;
		}
		/* 输入了非数字时清掉缓冲区，防止菜单被反复刷屏 */
		int c;
		while ((c = getchar()) != '\n' && c != EOF)
		{
		}
		return -1;
	}
	return select;
}

/*
 * 录入一名学生：头插法插入链表。
 * 头插法不需要遍历，O(1) 完成插入；代价是打印顺序与录入顺序相反。
 */
void entryStudent(List* list)
{
	Node* node = CreateNode();
	if (!node)
	{
		return;
	}

	printf("输入学生学号：");
	scanf("%llu", &node->stu.number);

	printf("输入学生姓名：");
	scanf("%31s", node->stu.name);

	printf("输入学生语文成绩：");
	scanf("%f", &node->stu.chinese);

	printf("输入学生数学成绩：");
	scanf("%f", &node->stu.math);

	printf("输入学生英语成绩：");
	scanf("%f", &node->stu.english);

	node->next = list->front;
	list->front = node;
	list->size++;
}

/* 遍历链表打印全部学生 */
void printStudent(List* list)
{
	printf("**********************************\n");
	printf("*\t欢迎使用学生管理系统\t *\n");
	printf("**********************************\n");
	printf("*学号 * 姓名 * 语文 * 数学 * 英语*\n");
	printf("**********************************\n");

	if (list->front == NULL)
	{
		printf("*            暂无学生            *\n");
		printf("**********************************\n");
		return;
	}

	Node* curNode = list->front;
	while (curNode != NULL)
	{
		printf("*%-4llu * %-4s * %-4.1f * %-4.1f * %-4.1f*\n",
			curNode->stu.number, curNode->stu.name,
			curNode->stu.chinese, curNode->stu.math, curNode->stu.english);
		curNode = curNode->next;
	}
}

/* 二进制存档：整块结构体直接写盘，体积小、读写快，但用记事本打不开 */
void saveStudent(List* list)
{
	FILE* fp = fopen("students.data", "wb");
	if (!fp)
	{
		perror("file open failed");
		return;
	}

	Node* curNode = list->front;
	while (curNode != NULL)
	{
		fwrite(&curNode->stu, sizeof(Student), 1, fp);
		curNode = curNode->next;
	}

	fclose(fp);
}

/* 文本存档：一行一名学生，字段用空白分隔，可以用记事本直接改 */
void saveStudentHuman(List* list)
{
	FILE* fp = fopen("student.txt", "w");
	if (!fp)
	{
		perror("file open failed");
		return;
	}

	Node* curNode = list->front;
	while (curNode != NULL)
	{
		fprintf(fp, "%-4llu %-4s %-4.1f %-4.1f %-4.1f\n",
			curNode->stu.number, curNode->stu.name,
			curNode->stu.chinese, curNode->stu.math, curNode->stu.english);
		curNode = curNode->next;
	}

	fclose(fp);
}

/* 从二进制存档读取，读到末尾或读失败就停 */
void readStudent(List* list)
{
	FILE* fp = fopen("students.data", "rb");
	if (!fp)
	{
		perror("file open failed");
		return;
	}

	Node* node = CreateNode();
	while (node != NULL && fread(&node->stu, sizeof(Student), 1, fp) == 1)
	{
		node->next = list->front;
		list->front = node;
		list->size++;
		node = CreateNode();
	}
	free(node);

	fclose(fp);
}

/* 从文本存档读取：scanf 的 %f 遇到空白会自动跳过，所以空格 / Tab 分隔都能读 */
void readStudentHuman(List* list)
{
	FILE* fp = fopen("student.txt", "r");
	if (!fp)
	{
		perror("file open failed");
		return;
	}

	while (!feof(fp))
	{
		Node* node = CreateNode();
		if (!node)
		{
			break;
		}

		if (5 != fscanf(fp, "%llu\t%31s\t%f\t%f\t%f",
			&node->stu.number, node->stu.name,
			&node->stu.chinese, &node->stu.math, &node->stu.english))
		{
			free(node);
			break;
		}

		node->next = list->front;
		list->front = node;
		list->size++;
	}

	fclose(fp);
}

/* 统计人数：演示链表遍历，同时也和 list->size 相互印证 */
void statisticsStudentCount(List* list)
{
	int count = 0;
	Node* curNode = list->front;
	while (curNode != NULL)
	{
		count++;
		curNode = curNode->next;
	}
	printf("学生总人数为：%d\n", count);
}

/*
 * 按学号或姓名查找。
 * 先把输入当成数字解析一次：解析成功说明用户想按学号查，
 * 解析失败（如输入的是姓名）number 会保持一个不可能出现的哨兵值。
 */
Node* findStudent(List* list)
{
	char buffer[32];
	printf("请输入要查找的学生学号或姓名>");
	scanf("%31s", buffer);

	unsigned long long number = (unsigned long long)-1;
	sscanf(buffer, "%llu", &number);

	Node* curNode = list->front;
	while (curNode != NULL)
	{
		if (strcmp(curNode->stu.name, buffer) == 0 ||
			curNode->stu.number == number)
		{
			return curNode;
		}
		curNode = curNode->next;
	}
	return NULL;
}

/* 修改：找到学生后只允许改三科成绩 */
void alterStudent(List* list)
{
	char buffer[32];
	printf("请输入要修改的学生学号或姓名>");
	scanf("%31s", buffer);

	unsigned long long number = (unsigned long long)-1;
	sscanf(buffer, "%llu", &number);

	Node* curNode = list->front;
	while (curNode != NULL)
	{
		if (strcmp(curNode->stu.name, buffer) == 0 ||
			curNode->stu.number == number)
		{
			printf("请输入要修改的学生的 语文成绩 数学成绩 英语成绩>");
			scanf("%f %f %f", &curNode->stu.chinese, &curNode->stu.math,
				&curNode->stu.english);
			printf("修改成功\n");
			break;
		}
		curNode = curNode->next;
	}

	if (curNode == NULL)
	{
		printf("没有找到该学生，修改失败\n");
	}
}

/* 删除：需要记住前驱节点，因为它要接管被删节点的 next */
void removeStudent(List* list)
{
	if (list->size == 0)
	{
		printf("暂无学生\n");
		return;
	}

	char buffer[32];
	printf("请输入要删除的学生学号或姓名>");
	scanf("%31s", buffer);

	unsigned long long number = (unsigned long long)-1;
	sscanf(buffer, "%llu", &number);

	Node* curNode = list->front;
	Node* prevNode = NULL;
	while (curNode != NULL)
	{
		if (strcmp(curNode->stu.name, buffer) == 0 ||
			curNode->stu.number == number)
		{
			break;
		}

		prevNode = curNode;
		curNode = curNode->next;
	}

	if (curNode)
	{
		if (prevNode == NULL)
		{
			/* 删除的是首节点 */
			list->front = curNode->next;
		}
		else
		{
			prevNode->next = curNode->next;
		}
		free(curNode);
		list->size--;
		printf("删除成功\n");
	}
	else
	{
		printf("没有找到该学生，删除失败\n");
	}
}
