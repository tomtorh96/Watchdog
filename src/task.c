/**********************
 * task.c
 * Author: Tomer Harel
 * Reviewer: Maor Zohar 
**********************/
#include <time.h>
#include <assert.h>
#include <stdlib.h>

#include "task.h"

#define SUCCESS (0)
#define FAILURE (1)
#define IS_ERROR_TIME(time) (0 > time)

struct Task {
    ilrd_uid_t task_id;
    size_t interval;
    time_t execution_time;
    int (*Action)(void* params);
    void* action_params;
    void (*Clean)(void* params);
    void* clean_params;
};

task_t* TaskCreate(int (*Action)(void* params), void* action_params,
                   void (*Clean)(void* params), void* clean_params,
                   int is_repeat, size_t interval)
{
	task_t* task = NULL;
	
	assert(NULL != Action);
	assert(NULL != Clean);
	
	task = (task_t*)malloc(sizeof(task_t));
	if (NULL == task)
	{
		return NULL;
	}
	
	task->task_id = UIDCreate();
	if (UIDIsSame(task->task_id,UIDBadUID))
	{
		free(task);
		task = NULL;
		return NULL;
	}
	
	task->Action = Action;
	task->action_params = action_params;
	task->Clean = Clean;
	task->clean_params = clean_params;
	
	task->interval = interval;
	if (TaskReschedule(task))
	{
		free(task);
		task = NULL;
		return NULL;
	}
		
	task->interval = is_repeat ? interval: 0;
	
	return task;
}
                  
void TaskDestroy(task_t* task)
{
	
	if (NULL != task && NULL != task->Clean)
	{
		task->Clean(task->clean_params);
		
		task->Clean = NULL;
		task->Action = NULL;
	}
	
	free(task);
	task = NULL;
}

ilrd_uid_t TaskGetUID(const task_t* task)
{
	assert(NULL != task);
	
	return task->task_id;
}

int TaskRun(task_t* task)
{
	int status = 0;
	assert(NULL != task);
	assert(NULL != task->Action);
	
	status = task->Action(task->action_params);
	if(SUCCESS != status)
	{
		task->interval = 0;
	}
	
	return status;
}

int TaskReschedule(task_t* task)
{
	assert(NULL != task);
	
	task->execution_time = time(NULL);
	if (IS_ERROR_TIME(task->execution_time))
	{
		return FAILURE;
	}
	
	task->execution_time += task->interval;
	return SUCCESS;
}

time_t TaskGetTimeToRun(const task_t* task)
{
	assert(NULL != task);
	
	return task->execution_time;
}

int TaskIsToRepeat(const task_t* task)
{
	assert(NULL != task);
	
	return !!task->interval;
}

int TaskIsMatch(const task_t* task1, const task_t* task2)
{
	assert(NULL != task1);
	assert(NULL != task2);
	
	return UIDIsSame(task1->task_id, task2->task_id);
}

int TaskCmpPriority(const task_t* task1, const task_t* task2)
{
	return difftime(task1->execution_time, task2->execution_time);
}
