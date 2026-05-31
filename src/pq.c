 /**********************
 * pq.c
 * Author: Tomer Harel
 * Reviewer: maxim shteingard
**********************/

#include <stdlib.h> /*malloc*/
#include <assert.h> /*assert*/

#include "pq.h" /*pq_t*/
#include "sorted_list.h" /*API, sorted_list_iter_t*/

struct PriorityQueue
{
    sorted_list_t* sorted_list;
};
 
pq_t* PQCreate(int (*CmpFunc)(const void* element1, 
										  const void* element2))
{
	pq_t* pq = NULL;
	
	assert(NULL != CmpFunc);
	
	pq = (pq_t*)malloc(sizeof(pq_t));
	if (NULL == pq)
	{
		return NULL;
	}
	
	pq->sorted_list = SortedListCreate(CmpFunc);
	if (NULL == pq->sorted_list)
	{
		free(pq);
		return NULL;
	}
	
	return pq;
}

void PQDestroy(pq_t* queue)
{
	assert(NULL != queue);
	assert(NULL != queue->sorted_list);
	
	SortedListDestroy(queue->sorted_list);
	
	queue->sorted_list = NULL;
	
	free(queue);
}

int PQEnqueue(pq_t* queue, void* data)
{
	sorted_list_iter_t end = {0};
	
	assert(NULL != queue);
	assert(NULL != queue->sorted_list);
	
	end = SortedListEnd(queue->sorted_list);
	
	return SortedListIterIsEqual(
			SortedListInsert(queue->sorted_list, data), end);
}

void* PQDequeue(pq_t* queue)
{
	assert(NULL != queue);
	assert(NULL != queue->sorted_list);
	assert(!PQIsEmpty(queue));
	
	return SortedListPopBack(queue->sorted_list);
}

void* PQPeek(const pq_t* queue)
{
	sorted_list_iter_t top_element = {0};
	
	assert(NULL != queue);
	assert(NULL != queue->sorted_list);
	assert(!PQIsEmpty(queue));
	
	top_element = SortedListPrev(SortedListEnd(queue->sorted_list));
	
	return SortedListIterGetData(top_element);
}

int PQIsEmpty(const pq_t* queue)
{
	assert(NULL != queue);
	assert(NULL != queue->sorted_list);
	
	return SortedListIsEmpty(queue->sorted_list);
}

size_t PQSize(const pq_t* queue)
{
	assert(NULL != queue);
	assert(NULL != queue->sorted_list);
	
	return SortedListSize(queue->sorted_list); 
}

void PQClear(pq_t* queue)
{
	assert(NULL != queue);
	assert(NULL != queue->sorted_list);
	
	
	while (!PQIsEmpty(queue))
	{
		PQDequeue(queue);
	}
}

void* PQErase(pq_t* queue, 
              int (*IsMatchFunc)(const void* data, const void* param), 
              const void* param)
	
{
	sorted_list_iter_t begin = {0};
	sorted_list_iter_t end = {0};
	sorted_list_iter_t found = {0};
	void* data = NULL;
	
	assert(NULL != queue);
	assert(NULL != queue->sorted_list);
	assert(!PQIsEmpty(queue));
	
	begin = SortedListBegin(queue->sorted_list);
	end = SortedListEnd(queue->sorted_list);
	
	found =  SortedListFindIf(begin, end, IsMatchFunc, param);
	
	if (SortedListIterIsEqual(found, end))
	{
		return NULL;
	}
	
	data = SortedListIterGetData(found);
	
	SortedListRemove(found);
	
	return data;
}
