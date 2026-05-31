/**********************
 * sched.c
 * Author: Tomer Harel
 * Reviewer: Maor Zohar
 **********************/

#include <assert.h>	   /*assert*/
#include <stdlib.h>	   /*malloc, free*/
#include <unistd.h>	   /*sleep*/
#include "pq.h"		   /*pq_t*/
#include "scheduler.h" /*sched_t*/
#include "task.h"	   /*task_t*/

#define MAX2(t1, t2) (t1 > t2 ? t1 : t2)
#define IS_ERROR_TIME(time) (0 > time)
#define TRUE (1)
#define FALSE (0)
#define IS_CURRENT_TASK_VALID(task) (NULL != task)

struct Scheduler
{
	pq_t* pq;
	int is_running;
	task_t* current_task;
};

static void GetCurrentTime(time_t* time);
static int GetWaitingTime(time_t current_time, time_t execution_time);
static int SchedCmpPriority(const void* element1, const void* element2);
static int SchedIsSameUID(const void* element1, const void* element2);
static int SchedPostAction(sched_t* sched, int status);

sched_t* SchedCreate(void)
{
	sched_t* sched = (sched_t*)malloc(sizeof(sched_t));
	if (NULL == sched)
	{
		return NULL;
	}

	sched->pq = PQCreate(SchedCmpPriority);
	if (NULL == sched->pq)
	{
		free(sched);
		return NULL;
	}

	sched->is_running = FALSE;
	sched->current_task = NULL;

	return sched;
}

void SchedDestroy(sched_t* sched)
{
	assert(NULL != sched);
	assert(NULL != sched->pq);

	SchedClear(sched);

	PQDestroy(sched->pq);

	sched->pq = NULL;
	sched->current_task = NULL;

	free(sched);
	sched = NULL;
}

ilrd_uid_t SchedAdd(sched_t* sched, int (*Action)(void* params),
					void* action_params, void (*Clean)(void* params),
					void* clean_params, int is_repeat, size_t interval)
{
	task_t* task = NULL;

	assert(NULL != sched);
	assert(NULL != Action);
	assert(NULL != Clean);

	task = TaskCreate(Action, action_params, Clean, clean_params, is_repeat,
					  interval);
	if (NULL == task)
	{
		return UIDBadUID;
	}

	if (PQEnqueue(sched->pq, task))
	{
		TaskDestroy(task);
		task = NULL;
		return UIDBadUID;
	}

	return TaskGetUID(task);
}

int SchedRemove(sched_t* sched, ilrd_uid_t uid)
{
	task_t* task = NULL;

	assert(NULL != sched);
	assert(!UIDIsSame(uid, UIDBadUID));

	if (IS_CURRENT_TASK_VALID(sched->current_task) &&
		UIDIsSame(uid, TaskGetUID(sched->current_task)))
	{
		task = sched->current_task;
		sched->current_task = NULL;
	}
	else
	{
		task = PQErase(sched->pq, SchedIsSameUID, &uid);
		if (NULL == task)
		{
			return NOT_FOUND;
		}
	}

	TaskDestroy(task);
	task = NULL;

	return SUCCESS;
}

int SchedRun(sched_t* sched)
{
	int status = EMPTY;
	time_t current_time = -1;
	int sleep_time = 0;

	assert(NULL != sched);

	sched->is_running = 1;

	while (sched->is_running && !SchedIsEmpty(sched))
	{
		GetCurrentTime(&current_time);
		if (0 > current_time)
		{
			SchedStop(sched);
			return ERROR;
		}

		sched->current_task = (task_t*)PQDequeue(sched->pq);

		sleep_time =
			GetWaitingTime(current_time, TaskGetTimeToRun(sched->current_task));
		while (0 < sleep_time)
		{
			sleep_time = sleep(sleep_time);
		}

		status = TaskRun(sched->current_task);
		if (IS_CURRENT_TASK_VALID(sched->current_task) &&
			ERROR == SchedPostAction(sched, status))
		{
			SchedStop(sched);
			return ERROR;
		}
	}
	status = !sched->is_running ? STOP : EMPTY;

	SchedStop(sched);

	return status;
}

void SchedStop(sched_t* sched)
{
	assert(NULL != sched);

	sched->is_running = FALSE;
}

size_t SchedSize(const sched_t* sched)
{
	assert(NULL != sched);

	return PQSize(sched->pq);
}

int SchedIsEmpty(const sched_t* sched)
{
	assert(NULL != sched);
	assert(NULL != sched->pq);

	return PQIsEmpty(sched->pq);
}

void SchedClear(sched_t* sched)
{
	task_t* task_to_remove = NULL;

	assert(NULL != sched);
	assert(NULL != sched->pq);

	while (!SchedIsEmpty(sched))
	{
		task_to_remove = (task_t*)PQDequeue(sched->pq);
		TaskDestroy(task_to_remove);
	}
	task_to_remove = NULL;
}

/*============================================================================*/
static int SchedPostAction(sched_t* sched, int status)
{
	if (SUCCESS != status || !TaskIsToRepeat(sched->current_task))
	{
		TaskDestroy(sched->current_task);
	}
	else
	{
		TaskReschedule(sched->current_task);
		if (PQEnqueue(sched->pq, sched->current_task))
		{
			TaskDestroy(sched->current_task);
			return ERROR;
		}
	}
	return SUCCESS;
}

static void GetCurrentTime(time_t* current_time)
{
	size_t index = 0;

	for (; index < 10 && IS_ERROR_TIME(*current_time); ++index)
	{
		time(current_time);
	}
}
static int GetWaitingTime(time_t current_time, time_t execution_time)
{
	double sleep = 0;

	sleep = difftime(execution_time, current_time);

	return MAX2((int)sleep, 0);
}

static int SchedCmpPriority(const void* element1, const void* element2)
{
	return 0 > TaskCmpPriority((task_t*)element1, (task_t*)element2);
}

static int SchedIsSameUID(const void* element1, const void* element2)
{
	return UIDIsSame(TaskGetUID((task_t*)element1),
					 TaskGetUID((task_t*)element2));
}
