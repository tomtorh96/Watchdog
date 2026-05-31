/**********************
 * test_task.c
 * Author: Tomer Harel
 * Reviewer: Maor Zohar 
**********************/

#include <stdio.h>/*printf*/
#include <stdlib.h>

#include "task.h"

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[1;33m"
#define COLOR_RESET   "\x1b[0m"

static int print(void* a);
static void clean(void* a);

static void TestCreateTask();
static void TestCreateAndRunTask();

int main()
{
	TestCreateTask();
	TestCreateAndRunTask();
	return 0;
}

static void TestCreateTask()
{
	int a = 5;
	task_t*	task = TaskCreate(print,&a,clean,&a,0,0);
	if (NULL == task)
	{
		printf("TestCreateTask ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		TaskDestroy(task);
		return;
	}
	
	printf("TestCreateTask ");
	printf(COLOR_GREEN"PASSED\n"COLOR_RESET);
	TaskDestroy(task);
}
static void TestCreateAndRunTask()
{
	int a = 5;
	time_t first_run;
	time_t second_run;
	
	task_t*	task = TaskCreate(print,&a,clean,&a,1,10);
	if (NULL == task)
	{
		printf("TestCreateAndRunTask ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		TaskDestroy(task);
		return;
	}
	
	first_run = TaskGetTimeToRun(task);
	
	TaskRun(task);
	
	TaskReschedule(task);
	
	second_run = TaskGetTimeToRun(task);
	
	if (0.001 < second_run - first_run)
	{
		printf("TestCreateAndRunTask ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("task wasn't reschedule\nexpected: %lu got %lu diffrents: %lu\n",
				first_run, second_run, second_run - first_run);
		TaskDestroy(task);
		return;
	}
	printf("TestCreateAndRunTask ");
	printf(COLOR_GREEN"PASSED\n"COLOR_RESET);
	TaskDestroy(task);
}
static int print(void* a)
{
	printf("printing %d\n",*(int*)a);
	return 0;
}

static void clean(void* a)
{
	printf("i am cleaning %d\n",*(int*)a);
}
