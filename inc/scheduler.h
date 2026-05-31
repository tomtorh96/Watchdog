/**********************
 * scheduler.h
 * Author: Tomer Harel
 * Reviewer: Maor Zohar
 **********************/

#ifndef __ILRD_SCHED_H__
#define __ILRD_SCHED_H__

#include "uid.h" /* time_t, uid typedef */

typedef enum
{
	ERROR = -1,
	EMPTY,
	STOP
} status_run;

typedef enum
{
	NOT_FOUND = -1,
	SUCCESS
} status_remove;

typedef struct Scheduler sched_t;

/**
 * Description: Creates a Scheduler.
 *
 * Arguments:  -
 *
 * Return:      Success - Pointer to a newly allocated scheduler,
 *              Failure - NULL.
 *
 * Complexity:  Time:  O(1)
 *              Space: O(n)
 */
sched_t* SchedCreate(void);

/**
 * Description: Free a Scheduler.
 *
 * Arguments:   scheduler - Pointer to a scheduler, must not be NULL.
 *
 * Return:      -
 *
 * Complexity:  Time:  O(n)
 *              Space: O(1)
 */
void SchedDestroy(sched_t* scheduler);

/**
 * Description: Adds task to the Scheduler.
 *
 * Arguments:  scheduler      - Pointer to scheduler to add to.
 *                              a valid address, must not be NULL.
 *              *Action       - Pointer to a function to execute,
 *                              a valid address, must not be NULL.
 *              action_params - Pointer to action params to pass
 *                              to the execution function
 *              *Clean        - Pointer to a clean up function,
 *                              a valid address. must not be NULL.
 *              clean_params  - Pointer to the clean params to pass
 *                              to the clean up function
 *              is_repeat     - Boolen value if task is repeatable
 *              interval      - Interval time in seconds for function
 *                              execution
 *
 * Return:      Success - ilrd_uid_t of inserted task
 *              Failure - ilrd_uid_t of UIDBadUID and stops the execution
 *
 * Complexity:  Time:  O(1)
 *              Space: O(n)
 */
ilrd_uid_t SchedAdd(sched_t* scheduler, int (*Action)(void* params),
					void* action_params, void (*Clean)(void* params),
					void* clean_params, int is_repeat, size_t interval);

/**
 * Description: Removes a task, by uid, from the scheduler.
 *
 * Arguments:   scheduler - pointer to scheduler to remove from.
 *                          a valid address, must not be NULL.
 *              uid       - uid of a task to remove from scheduler.
 *
 * Return:      Success   - (0) Successfuly removed.
 *              Not Found - (-1) There wasn't a task in scheduler
 *                          with the uid given.
 *
 * Complexity:  Time:  O(n)
 *              Space: O(1)
 */
int SchedRemove(sched_t* scheduler, ilrd_uid_t uid);

/**
 * Description: Runs the scheduler loop and executes scheduled tasks until
 *              there are no more tasks to execute, or stop function is called.
 *
 *
 * Arguments:   scheduler - pointer to scheduler to run.
 *                          a valid address, must not be NULL.
 *
 * Return:      Empty     - (0) no more tasks to run.
 *              Error     - (-1) scheduler internal error.
 *              Stop      - (1) stop function is called.
 *
 * Complexity:  Time:  O(n)
 *              Space: O(1)
 */
int SchedRun(sched_t* scheduler);

/**
 * Description: Stop running the scheduler after the currently running task
 * completes.
 *
 * Arguments:   scheduler - pointer to scheduler.
 *                          a valid address, must not be NULL.
 *
 * Return:      -
 *
 * Complexity:  Time:  O(1)
 *              Space: O(1)
 */
void SchedStop(sched_t* scheduler);

/**
 * Description: Returns the number of elements of the scheduler.
 *
 * Arguments:   scheduler - Pointer to scheduler, must not be NULL.
 *
 * Return:      size_t - number of elements.
 *
 * Complexity:  Time:  O(1)
 *			    Space: O(1)
 **/
size_t SchedSize(const sched_t* scheduler);

/**
 * Description: Checks if scheduler is empty.
 *
 * Arguments:   scheduler - Pointer to scheduler, must not be NULL.
 *
 * Return:      TRUE  (1) If scheduler is empty,
 *              FALSE (0) If isn't empty.
 *
 * Complexity:  Time:  O(1)
 *			    Space: O(1)
 **/
int SchedIsEmpty(const sched_t* scheduler);

/**
 * Description: Clears the scheduler from all it's tasks.
 *
 * Arguments:   scheduler - pointer to scheduler to clear.
 *                          a valid address, must not be NULL.
 *
 * Return:      -
 *
 * Complexity:  Time:  O(n)
 *              Space: O(1)
 */
void SchedClear(sched_t* scheduler);

#endif /* __ILRD_SCHED_H__ */
