/**********************
 * dll.c
 * Author: Tomer Harel
 * Reviewer: Shahar Lasri 
**********************/

#include <stdlib.h> /*malloc*/
#include <assert.h> /*assert*/

#include "dll.h" /*typedef, api*/

#define NOT_SAME_NODE(it1, it2) (0 == DLLIsSameIter(it1, it2))

typedef struct DLLNode node_t;

struct DLLNode {
    void* data;
    node_t* next;
    node_t* prev;
};

struct DLL {
    node_t head;
    node_t tail;
};

static void JoinNodes(dlist_iter_t from, dlist_iter_t to);
static node_t* IterToDLLNode(dlist_iter_t iter); 
static dlist_iter_t DLLNodeToIter(node_t* node);
static int CountElements(void* data, void* number_of_elements);

dll_t* DLLCreate(void)
{
	dll_t* list = (dll_t*)malloc(sizeof(dll_t));
	if (NULL == list)
	{
		return NULL;
	}
	
	list->head.next = &list->tail;
	list->head.prev = NULL;
	list->head.data = list;
	
	list->tail.prev = &list->head;
	list->tail.next = NULL;
	list->tail.data = list;
	
	return list;
}

void DLLDestroy(dll_t* list)
{
	dlist_iter_t current = NULL;
	dlist_iter_t end = NULL;
	
	assert(NULL != list);
	
	current = DLLBegin(list);
	end = DLLEnd(list);
	
	while(0 == DLLIsSameIter(current, end))
	{
		current = current->next;
		
		free(current->prev);
	}
	
	free(list);
}

dlist_iter_t DLLBegin(const dll_t* list)
{
	assert(NULL != list);

	return DLLNodeToIter(list->head.next);	
}

dlist_iter_t DLLEnd(const dll_t* list)
{
	assert(NULL != list);

	return DLLNodeToIter((node_t*)&list->tail);
}

dlist_iter_t DLLNext(dlist_iter_t current)
{
	assert(NULL != IterToDLLNode(current));
	assert(NULL != IterToDLLNode(current)->next);
	
	return DLLNodeToIter(IterToDLLNode(current)->next);
}

dlist_iter_t DLLPrev(dlist_iter_t current)
{
	assert(NULL != IterToDLLNode(current));
	assert(NULL != IterToDLLNode(current)->prev);
	
	return DLLNodeToIter(IterToDLLNode(current)->prev);
}

int DLLIsSameIter(dlist_iter_t iter1, dlist_iter_t iter2)
{
	assert(NULL != IterToDLLNode(iter1));
	assert(NULL != IterToDLLNode(iter2));
		
	return IterToDLLNode(iter1) == IterToDLLNode(iter2);
}

void* DLLGetData(dlist_iter_t iter)
{
	assert(NULL != IterToDLLNode(iter));
	
	return IterToDLLNode(iter)->data;
}

dlist_iter_t DLLInsert(dll_t* list, dlist_iter_t where, void* data)
{
	node_t* new_node = NULL;
	
	assert(NULL != list);
	assert(NULL != IterToDLLNode(where));
	
	new_node = (node_t*)malloc(sizeof(node_t));
	if (NULL == new_node)
	{
		return DLLEnd(list);
	}
	 
	new_node->data = data;
	
	where->prev->next = new_node;
	
	new_node->prev = where->prev;
	new_node->next = where;
	
	where->prev = new_node;
	
	return DLLNodeToIter(new_node);
}

dlist_iter_t DLLRemove(dlist_iter_t iter)
{
	dlist_iter_t target_iter = iter;
	
	assert(NULL != IterToDLLNode(iter));
	
	iter->prev->next = iter->next;
	iter->next->prev = iter->prev;

	iter = iter->next;

	free(target_iter);
	target_iter = NULL;
	
	return iter;
}

dlist_iter_t DLLPushFront(dll_t* list, void* data)
{
	assert(NULL != list);
	
	return DLLInsert(list,DLLBegin(list),data);
}

dlist_iter_t DLLPushBack(dll_t* list, void* data)
{
	assert(NULL != list);
	
	return DLLInsert(list,DLLEnd(list),data);
}

void* DLLPopFront(dll_t* list)
{
	void* data = NULL;
	dlist_iter_t first_element = NULL;

	assert(NULL != list);
	assert(FALSE == DLLIsEmpty(list));
	
	first_element = DLLNodeToIter(&list->head)->next;
	data = first_element->data;
	
	DLLRemove(first_element);
	
	return data;
}

void* DLLPopBack(dll_t* list)
{
	void* data = NULL;
	dlist_iter_t last_element = NULL;

	assert(NULL != list);
	assert(FALSE == DLLIsEmpty(list));
	
	last_element = DLLEnd(list)->prev;
	data = last_element->data;
	
	DLLRemove(last_element);
	
	return data;
}

size_t DLLCount(const dll_t* list)
{
	size_t count = 0;

	assert(NULL != list);
	
	DLLForEach(DLLBegin(list), DLLEnd(list), CountElements, &count);
	
	return count;
}

int DLLIsEmpty(const dll_t* list)
{
	assert(NULL != list);
	
	return DLLBegin(list) == DLLEnd(list);
}

dlist_iter_t DLLFind(dlist_iter_t from, dlist_iter_t to, 
                     int (*IsMatch)(const void* list_data, const void* param), 
                     const void* param)
{
	assert(NULL != IterToDLLNode(from));
	assert(NULL != IterToDLLNode(to));
	assert(NULL != IsMatch);
	
	while(FALSE == DLLIsSameIter(from, to))
	{
		if (TRUE == IsMatch(from->data, param))
		{
			return from;
		}
		
		from = DLLNext(from);
	}
	
	return to;
}

int DLLMultiFind(dlist_iter_t from, dlist_iter_t to,
                 int (*IsMatch)(const void* list_data, const void* param),
                 const void* param, 
                 dll_t* dest_list)
{	
	int status = -1;
	
	assert(NULL != IterToDLLNode(from));
	assert(NULL != IterToDLLNode(to));
	assert(NULL != IsMatch);
	assert(NULL != dest_list);
	
	while(FALSE == DLLIsSameIter(from, to))
	{
		from = DLLFind(from, to, IsMatch, param);
		if (FALSE == DLLIsSameIter(from, to))
		{
			if (TRUE == DLLIsSameIter(DLLPushBack(dest_list,DLLGetData(from)),
			 DLLEnd(dest_list)))
			{
				return ERROR;
			}
			
			status = 0;
			from = from->next;
		}		
	}
	
	return status;
}

int DLLForEach(dlist_iter_t from, dlist_iter_t to, 
               int (*ActionFunc)(void* list_data, void* param), 
               void* param)
{
	int status = SUCCESS;
	
	assert(NULL != IterToDLLNode(from));
	assert(NULL != IterToDLLNode(to));
	assert(NULL != ActionFunc);
	
	while(FALSE == DLLIsSameIter(from, to))
	{
		status = ActionFunc(from->data, param);
		if (SUCCESS != status)
		{
			return status;
		}
		
		from = DLLNext(from);
	}
	
	return SUCCESS;
}

dlist_iter_t DLLSplice(dlist_iter_t where, dlist_iter_t from, dlist_iter_t to)
{
	node_t* prev_element = NULL; 
	node_t* last_element = NULL;
	
	assert(NULL != IterToDLLNode(from));
	assert(NULL != IterToDLLNode(to));
	assert(NULL != IterToDLLNode(where));
	assert(FALSE == DLLIsSameIter(from, to));
	
	prev_element = IterToDLLNode(where)->prev; 
	last_element = IterToDLLNode(to)->prev;
	
	JoinNodes(from->prev,last_element->next);
	
	JoinNodes(last_element, where);
	
	JoinNodes(prev_element, from);

	return last_element;
}

/*============================================================================*/
static node_t* IterToDLLNode(dlist_iter_t iter)
{	
	assert(NULL != iter);
	
	return (node_t*) iter;
}

static dlist_iter_t DLLNodeToIter(node_t* node)
{
	assert(NULL != node);
	
	return (dlist_iter_t) node;
}

static int CountElements(void* data, void* number_of_elements)
{
	assert(NULL != number_of_elements);
	
	(void)data;
		
	*(size_t*)number_of_elements += 1;
	
	return SUCCESS;
}

static void JoinNodes(dlist_iter_t from, dlist_iter_t to)
{
	from->next = to;
	to->prev = from;
}
