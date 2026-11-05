#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "student_manager.h"

/*
 * 暂停 / 清屏在两个平台上的命令不同：
 * Windows 用 pause / cls，类 Unix 系统用 read / clear。
 * 统一封成两个小函数，主流程里就不必关心平台差异了。
 */
static void pauseScreen(void)
{
#ifdef _WIN32
	system("pause");
#else
	int c;
	printf("按回车键继续...");
	fflush(stdout);
	while ((c = getchar()) != '\n' && c != EOF)
	{
	}
#endif
}

static void clearScreen(void)
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

int main(void)
{
	/* 链表头先全部清零：front = NULL，size = 0 */
	List list = { 0 };
	memset(&list, 0, sizeof(List));

	bool running = true;

	while (running)
	{
		MenuOptions m = (MenuOptions)menu();

		switch (m)
		{
		case Quit:
			running = false;
			break;
		case Entry:
			entryStudent(&list);
			break;
		case Print:
			printStudent(&list);
			break;
		case Save:
			/*
			 * 这里用「文本格式」存档：文件可以直接用记事本打开查看和修改。
			 * 库里另外提供了二进制版本的 saveStudent / readStudent，
			 * 需要更紧凑、读写更快的存档时可以在主流程里替换成它们。
			 */
			saveStudentHuman(&list);
			printf("保存成功\n");
			break;
		case Read:
			readStudentHuman(&list);
			printf("读取完成，当前共 %d 名学生\n", list.size);
			break;
		case Statistics:
			statisticsStudentCount(&list);
			break;
		case Find:
		{
			Node* node = findStudent(&list);
			if (!node)
			{
				printf("没有找到查询的学生\n");
			}
			else
			{
				printf("学号\t姓名\t语文\t数学\t英语\n");
				printf("%llu\t%s\t%.1f\t%.1f\t%.1f\n",
					node->stu.number, node->stu.name, node->stu.chinese,
					node->stu.math, node->stu.english);
			}
		}
			break;
		case Alter:
			alterStudent(&list);
			break;
		case Remove:
			removeStudent(&list);
			break;
		default:
			printf("无效的选择，请重新输入！\n");
			break;
		}

		if (running)
		{
			pauseScreen();
		}
		clearScreen();
	}

	/* 退出前落盘，避免录入的数据丢失 */
	saveStudent(&list);
	saveStudentHuman(&list);

	return 0;
}
