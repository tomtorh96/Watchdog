/**********************
 * task.h
 * Author: Tomer Harel
 * Reviewer: Maor Zohar 
**********************/

#ifndef __ILRD_TASK_H__
#define __ILRD_TASK_H__

#include "uid.h" /* declarations, uid typeded */

typedef struct Task task_t;

/**
 * Description: Creates a Task.
 *
 * Arguments:   Action - Function pointer to the task's execution logic.
 *              action_params - Arguments to be passed to the Action function.
 *              Clean - Function pointer to a cleanup routine.
 *              clean_params - Arguments for the Clean function.
 *              is_repeat - Boolean flag that say if need to reapet task.
 *              interval - Time in seconds until need to start run the func.
 *
 * Return:      Success - Pointer to a newly allocated task,
 *              Failure - NULL.
 *
 * Complexity:  Time:  O(1)
 *              Space: O(1)
 */
task_t* TaskCreate(int (*Action)(void* params), void* action_params,
                   void (*Clean)(void* params), void* clean_params,
                   int is_repeat, size_t interval);

 /**
 * Description: Free a Task.
 *
 * Arguments:   task - Pointer to a task.
 *
 * Return:      -
 *
 * Complexity:  Time:  O(n)
 *              Space: O(1)
 */                   
void TaskDestroy(task_t* task);

/**
 * Description:  Get the task uid.
 *
 * Arguments:    task - Pointer to a valid task. 
 *						Must not be NULL.
 *
 * Return:       The uid of the task. 
 *
 * Complexity:   Time:  O(1) 
 * 				 Space: O(1)
 */
ilrd_uid_t TaskGetUID(const task_t* task);

/**
 * Description:  Executes the action associated with the given task.
 *
 * Arguments:    task - Pointer to a valid task to be executed. 
 *						Must not be NULL.
 *
 * Return:       The return value of the task's action function. 
 *
 * Complexity:   Time:  O(1) 
 * 				 Space: O(1)
 */
int TaskRun(task_t* task);

/**
 * Description:  Update the next execution time of the task.
 *
 * Arguments:    task  - Pointer to a valid task to be executed. 
 *						 Must not be NULL.
 *
 * Return:       Success  - (0) task was updataed successfuly.
 *               Failed   - (1) failed to update the task.
 * 
 * Complexity:   Time:  O(1) 
 * 				 Space: O(1)
 */
int TaskReschedule(task_t* task);

/**
 * Description:  Get the execution time of the task.
 *
 * Arguments:    task - Pointer to a valid task to be executed. 
 *						Must not be NULL.
 *
 * Return:       time_t of when to execute the task. 
 *
 * Complexity:   Time:  O(1) 
 * 				 Space: O(1)
 */
time_t TaskGetTimeToRun(const task_t* task);

/**
 * Description:  Get boolen value if task is repeatable .
 *
 * Arguments:    task  - Pointer to a valid task to be executed. 
 *						 Must not be NULL.
 *
 * Return:       True  - (1) task is repeatable 
 *               False - (0) task is not repeatable
 * 
 * Complexity:   Time:  O(1) 
 * 				 Space: O(1)
 */
int TaskIsToRepeat(const task_t* task);

/**
 * Description:  Checks if two tasks are equal based on their Unique ID (UID).
 *
 * Arguments:    task1 - Pointer to a valid task, must not be NULL.
 *               task2 - Pointer to a valid task, must not be NULL.
 *               
 * Return:       TRUE  - (1) - If both tasks have the same UID.
 *               FALSE - (0) - Otherwise.
 *
 * Complexity:   Time:  O(1)
 *			     Space: O(1)
**/
int TaskIsMatch(const task_t* task1, const task_t* task2);

/**
 * Description:  Compares two tasks based on their scheduled execution time.
 *
 * Arguments:    task1 - Pointer to a valid task, must not be NULL.
 *               task2 - Pointer to a valid task, must not be NULL.
 *               
 * Return:       0 if both tasks are scheduled for the same time.
 *				 < 0 if task1 is scheduled earlier than task2.
 * 				 > 0 if task1 is scheduled later than task2. 
 *
 * Complexity:   Time:  O(1)
 *			     Space: O(1)
**/
int TaskCmpPriority(const task_t* task1, const task_t* task2);

#endif /* __ILRD_TASK_H__ */ 
