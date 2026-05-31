 /**********************
 * pq.h
 * Author: Tomer Harel
 * Reviewer: maxim shteingard
**********************/
#ifndef __ILRD_PRIORITY_QUEUE_H__
#define __ILRD_PRIORITY_QUEUE_H__

#include <stddef.h> /*size_t*/

typedef struct PriorityQueue pq_t;

 /**
 * Description: Creates a priority queue.
 *
 * Arguments:   CmpFunc - comparison function between two elements.
 *                        a valid function address, must not be NULL.
 *                        Arguments: element1 - element for PriorityQueue.
 *                                   element2 - element for PriorityQueue.
 *                        Returns:  if same (0),
 *   								if element1 > element2 (positive), 
 *   								if element1 < element2 (negative).
 *
 * Return:      Success - Pointer to a newly allocated priority queue,
 *              Failure - NULL.
 *
 * Complexity:  Time:  O(1)
 *              Space: O(n)
 */
pq_t* PQCreate(int (*CmpFunc)(const void* element1, const void* element2));

 /**
 * Description: Free a priority queue.
 *
 * Arguments:   queue - Pointer to a priority queue, must not be NULL.
 *
 * Return:      -
 *
 * Complexity:  Time:  O(n)
 *              Space: O(1)
 */
void PQDestroy(pq_t* queue);

 /**
 * Description: Adds a new element to the priority queue.
 *
 * Arguments:   queue - Pointer to a priority queue, must not be NULL.
 *              data  - Pointer to data to be inserted. 
 *
 * Return:    	SUCCESS (0) on success, 
 *              FAILED  (1) if memory allocation failed.
 *
 * Complexity:  Time:  O(n)
 *              Space: O(1)
 */
int PQEnqueue(pq_t* queue, void* data);

 /**
 * Description: Removes an element from the priority queue with 
 *              the highest priority.
 *
 * Arguments:   queue - Pointer to a priority queue, must not be NULL, 
 *                      queue must not be empty.
 *
 * Return:    	void* - Data of the element with the highest priority 
 *                      in the priority queue.
 *
 * Complexity:  Time:  O(1)
 *              Space: O(1)
 */
void* PQDequeue(pq_t* queue); 

 /**
 * Description: Returns the highest priority element in the priority queue.
 *
 * Arguments:   queue - Pointer to a Priority Queue, must not be NULL, 
 *                      queue must not be empty.
 *
 * Return:    	void* - Pointer to the element with the highest priority 
 *                      in the priority queue.
 *
 * Complexity:  Time:  O(1)
 *              Space: O(1)
 */
void* PQPeek(const pq_t* queue);

/**
 * Description: Checks if priority queue is empty.
 *
 * Arguments:   queue - Pointer to priority queue, must not be NULL.
 *               
 * Return:      TRUE  (1) If queue is empty, 
 *              FALSE (0) If isn't empty.
 *
 * Complexity:  Time:  O(1)
 *			    Space: O(1)
**/
int PQIsEmpty(const pq_t* queue);

/**
 * Description: Returns the number of elements of the priority queue.
 *
 * Arguments:   queue - Pointer to priority queue, must not be NULL.
 *               
 * Return:      size_t - number of elements.
 *
 * Complexity:  Time:  O(1)
 *			    Space: O(1)
**/
size_t PQSize(const pq_t* queue);

/**
 * Description: Dqueues all elements in the Priority Queue.
 *
 * Arguments:   queue - Pointer to priority queue, must not be NULL.
 *               
 * Return:      -
 *
 * Complexity:  Time:  O(n)
 *			    Space: O(1)
**/
void PQClear(pq_t* queue);

/**
 * Description: Removes the first element that satisfies a given condition.
 *
 * Arguments:   queue       - Pointer to a priority queue, must not be NULL.
 *                            queue must not be empty.
 *              IsMatchFunc - Function pointer to a valid matching function, 
 *                            must not be NULL.
 *                            Arguments: data  - data from priority queue.
 *                                       param - data from .
 *                            Returns:   TRUE (1) If the elements matches, 
 *                                       FALSE (0) otherwise.
 *              param       - Additional parameter to be passed to 
 *                            the match function.
 *
 * Return:      On success - Pointer to the data of the removed element.
 *              On failure - Returns NULL.
 *
 * Complexity:  Time:  O(n)
 *              Space: O(1)
 */
void* PQErase(pq_t* queue, 
              int (*IsMatchFunc)(const void* data, const void* param), 
              const void* param);

#endif /* __ILRD_PRIORITY_QUEUE_H__ */



