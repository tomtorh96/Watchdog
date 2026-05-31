/**********************
 * test_sched.c
 * Author: Tomer Harel
 * Reviewer: Maor Zohar 
**********************/

#include <stdio.h>/*printf*/
#include <unistd.h>/*sleep*/

#include "scheduler.h"/*API, sched_t*/

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[1;33m"
#define COLOR_RESET   "\x1b[0m"

static int print(void* param);
static int stoping(void* param);
static void cleanup(void* param);
static int Removing(void* param);

static void TestCreateAndDestory();
static void TestCreateAndRunTasks();
static void TestCreateAndRunTasksAndStop();
static void TestCreateAndTestWithDelay();

struct Param
{
	sched_t* sched;
	ilrd_uid_t uid;
};

int main()
{
	TestCreateAndDestory();
	TestCreateAndRunTasks();
	TestCreateAndRunTasksAndStop();
	TestCreateAndTestWithDelay();
	return 0;
}

static void TestCreateAndDestory()
{
	sched_t* sched = SchedCreate();
	if (NULL == sched)
	{
		printf("TestCreateAndDestory ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("problem creating Scheduler\n");
		return;
	}
	
	if (!SchedIsEmpty(sched))
	{
		printf("TestCreateAndDestory ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("Scheduler is empty\n");
		SchedDestroy(sched);
		return;
	}
	
	printf("TestCreateAndDestory ");
	printf(COLOR_GREEN"PASSED\n"COLOR_RESET);
	SchedDestroy(sched);
}

static void TestCreateAndRunTasks()
{
	size_t size = 3;
	size_t index = 0;
	size_t time[] = {2,4,6};
	int status = 0;
	char* lines[] = {"this","is","sparta!"};
	
	sched_t* sched = SchedCreate();
	if (NULL == sched)
	{
		printf("TestCreateAndRunTasks ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("problem creating Scheduler\n");
		return;
	}
	
	for (; index < size; ++index)
	{
		if (UIDIsSame(SchedAdd(sched, print,lines[index],cleanup,NULL,0,
							time[index]), UIDBadUID))
		{
			printf("TestCreateAndRunTasks ");
			printf(COLOR_RED"FAILED\n"COLOR_RESET);
			printf("problem creating task of %s\n",lines[index]);
			SchedDestroy(sched);
			return;
		}
	}
		
	status = SchedRun(sched);
	if (EMPTY != status)
	{
		printf("TestCreateAndRunTasks ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("incorrent end status got %d\n",status);
		SchedDestroy(sched);
		return;
	}
	
	printf("TestCreateAndRunTasks ");
	printf(COLOR_GREEN"PASSED\n"COLOR_RESET);
	SchedDestroy(sched);
}

static void TestCreateAndRunTasksAndStop()
{
	size_t size = 3;
	size_t index = 0;
	size_t time[] = {2,4,6};
	int status = 0;
	char* lines[] = {"this","is","sparta!"};
	
	sched_t* sched = SchedCreate();
	if (NULL == sched)
	{
		printf("TestCreateAndRunTasksAndStop ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("problem creating Scheduler\n");
		return;
	}
	
	for (; index < size; ++index)
	{
		if (UIDIsSame(SchedAdd(sched, print,lines[index],cleanup,NULL,0,
							time[index]), UIDBadUID))
		{
			printf("TestCreateAndRunTasksAndStop ");
			printf(COLOR_RED"FAILED\n"COLOR_RESET);
			printf("problem creating task of %s\n",lines[index]);
			SchedDestroy(sched);
			return;
		}
	}
	
	SchedAdd(sched, stoping,sched,cleanup,NULL,0,3);
	
	status = SchedRun(sched);
	if (STOP != status)
	{
		printf("TestCreateAndRunTasksAndStop ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("incorrent end status got %d\n",status);
		SchedDestroy(sched);
		return;
	}
	
	if (2 != SchedSize(sched))
	{
		printf("TestCreateAndRunTasksAndStop ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("expected %d got %lu",2,SchedSize(sched));
		SchedDestroy(sched);
		return;
	}
	
	SchedClear(sched);
	if (!SchedIsEmpty(sched))
	{
		printf("TestCreateAndRunTasksAndStop ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("Scheduler is empty\n");
		SchedDestroy(sched);
		return;
	}
	
	printf("TestCreateAndRunTasksAndStop ");
	printf(COLOR_GREEN"PASSED\n"COLOR_RESET);
	SchedDestroy(sched);
}

static void TestCreateAndTestWithDelay()
{
	size_t size = 3;
	size_t index = 0;
	ilrd_uid_t uid = UIDBadUID;
	size_t time[] = {20,27,30};
	char* lines[] = {"this","is","sparta!"};
	struct Param my_param;
	struct Param my_param2;
	sched_t* sched = SchedCreate();
	if (NULL == sched)
	{
		printf("TestCreateAndTestWithDelay ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("problem creating Scheduler\n");
		return;
	}
	
	for (; index < size; ++index)
	{
		if (1 == index)
		{
			uid = SchedAdd(sched, print,lines[index],cleanup,NULL,1,
							time[index]);
							
			if (UIDIsSame(uid, UIDBadUID))
			{
				printf("TestCreateAndTestWithDelay ");
				printf(COLOR_RED"FAILED\n"COLOR_RESET);
				printf("problem creating task of %s\n",lines[index]);
				SchedDestroy(sched);
				return;
			}
							
		}
		else if (UIDIsSame(SchedAdd(sched, print,lines[index],cleanup,NULL,1,
							time[index]), UIDBadUID))
		{
			printf("TestCreateAndTestWithDelay ");
			printf(COLOR_RED"FAILED\n"COLOR_RESET);
			printf("problem creating task of %s\n",lines[index]);
			SchedDestroy(sched);
			return;
		}
	}
	
	SchedAdd(sched, stoping,sched,cleanup,NULL,0,31);
	
	my_param2.sched = sched;
	my_param2.uid = uid;
	SchedAdd(sched, Removing,&my_param2,cleanup,NULL,0,26);
	
	my_param.sched = sched;
	my_param.uid = SchedAdd(sched, Removing,&my_param,cleanup,NULL,0,26);
	
	printf("sleeping\n");
	sleep(21);
	printf("running\n");
	
	SchedRun(sched);
	
	if (2 != SchedSize(sched))
	{
		printf("TestCreateAndRunTasksAndStop ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("expected %d got %lu",2,SchedSize(sched));
		SchedDestroy(sched);
		return;
	}
	
	printf("TestCreateAndTestWithDelay ");
	printf(COLOR_GREEN"PASSED\n"COLOR_RESET);
	SchedDestroy(sched);
}
static int print(void* param)
{
	printf("%s\n",(char*)param);
	return 0;
}

static int stoping(void* param)
{
	SchedStop((sched_t*)param);
	return 0;
}

static void cleanup(void* param)
{
	(void)param;
}

static int Removing(void* param)
{
	struct Param my_param = *(struct Param*)(param);
	
	SchedRemove(my_param.sched, my_param.uid);
	
	return 0;
}
