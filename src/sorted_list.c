 /**********************
 * sorted_list.c
 * Author: Tomer Harel
 * Reviewer: Yuval Chen
**********************/

#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc*/

#include "sorted_list.h" /*typedef SortedList SortedListIter, API*/

#define UNUSED(sorted_list) ((void)sorted_list)

static sorted_list_iter_t CreateSortedListIter(dlist_iter_t iter,
											   const sorted_list_t* list);
static int CmpToInsert(const void* a, const void* b);
static int CmpToFind(const void* a, const void* b);
#ifndef NDEBUG
static int FromSameList(sorted_list_iter_t iter1, sorted_list_iter_t iter2);
#endif/*NDEBUG*/

typedef struct DLLNode node_t;

struct SortedList {
    dll_t* list;
    int (*CmpFunc)(const void* element1, const void* element2);
};

typedef struct SortedElement 
{
	int (*InsertionPointFunc)(const void* element1, const void* element2);
	const void* param;

}sorted_element_t;

sorted_list_t* SortedListCreate(int (*CmpFunc)(const void* element1,
								const void* element2))
{
	sorted_list_t* slist = NULL;
	
	assert(NULL != CmpFunc);
	
	slist = (sorted_list_t*)malloc(sizeof(sorted_list_t));
	if (NULL == slist)
	{
		return NULL;
	}
	
	slist->list = DLLCreate();
	if (NULL == slist->list)
	{
		free(slist);
		slist = NULL;
		
		return NULL;
	}
	
	slist->CmpFunc = CmpFunc;
	
	return slist;
}

void SortedListDestroy(sorted_list_t* sorted_list)
{
	assert(NULL != sorted_list);
	
	DLLDestroy(sorted_list->list);
	
	sorted_list->list = NULL;
	sorted_list->CmpFunc = NULL;
	
	free(sorted_list);
	
	sorted_list = NULL;
}


sorted_list_iter_t SortedListBegin(const sorted_list_t* list)
{
	assert(NULL != list);
	assert(NULL != list->list);

	return CreateSortedListIter(DLLBegin(list->list),list);
}

sorted_list_iter_t SortedListEnd(const sorted_list_t* list)
{
	assert(NULL != list);
	assert(NULL != list->list);

	return CreateSortedListIter(DLLEnd(list->list),list);
}

sorted_list_iter_t SortedListNext(sorted_list_iter_t current)
{
	assert(NULL != current.inner_iter);
	assert(FALSE == SortedListIterIsEqual(current,SortedListEnd(current.list)));
	
	current.inner_iter = DLLNext(current.inner_iter);
	
	return current;
}

sorted_list_iter_t SortedListPrev(sorted_list_iter_t current)
{
	assert(NULL != current.inner_iter);
	assert(FALSE == SortedListIterIsEqual(current,SortedListBegin(current.list)));
	
	current.inner_iter = DLLPrev(current.inner_iter);
	
	return current;
}

int SortedListIterIsEqual(sorted_list_iter_t iter1, sorted_list_iter_t iter2)
{
	assert(NULL != iter1.inner_iter);
	assert(NULL != iter2.inner_iter);

	return DLLIsSameIter(iter1.inner_iter, iter2.inner_iter);
}

void* SortedListIterGetData(sorted_list_iter_t iter)
{
	assert(NULL != iter.inner_iter);
	assert(NULL != iter.list);
	assert(FALSE == SortedListIterIsEqual(iter,SortedListEnd(iter.list)));
	
	return DLLGetData(iter.inner_iter);
}

sorted_list_iter_t SortedListInsert(sorted_list_t* sorted_list, void* data)
{
	sorted_list_iter_t where = {0};
	sorted_element_t sort_element = {NULL,NULL};
	
	assert(NULL != sorted_list);
	assert(NULL != sorted_list->list);
	
	where = CreateSortedListIter(DLLBegin(sorted_list->list),sorted_list);
	
	sort_element.param = data;
	sort_element.InsertionPointFunc = sorted_list->CmpFunc;
	
	where.inner_iter = DLLFind(where.inner_iter, SortedListEnd(sorted_list).inner_iter, 
					CmpToInsert, &sort_element);
					
	where.inner_iter = DLLInsert(sorted_list->list, where.inner_iter, data);

	return where;
}

sorted_list_iter_t SortedListRemove(sorted_list_iter_t iter)
{
	assert(NULL != iter.inner_iter);
	assert(FALSE == SortedListIterIsEqual(iter,SortedListEnd(iter.list)));
	
	iter.inner_iter = DLLRemove(iter.inner_iter);
		
	return iter;
}

void* SortedListPopFront(sorted_list_t* list)
{
	assert(NULL != list);
	assert(NULL != list->list);
	assert(TRUE != SortedListIsEmpty(list));
	
	return DLLPopFront(list->list);
}

void* SortedListPopBack(sorted_list_t* list)
{	
	assert(NULL != list);
	assert(NULL != list->list);
	assert(TRUE != SortedListIsEmpty(list));
	
	return DLLPopBack(list->list);
}

size_t SortedListSize(const sorted_list_t* sorted_list)
{
	assert(NULL != sorted_list);
	assert(NULL != sorted_list->list);
	
	return DLLCount(sorted_list->list);
}

int SortedListIsEmpty(const sorted_list_t* sorted_list)
{
	assert(NULL != sorted_list);
	assert(NULL != sorted_list->list);
	
	return DLLIsEmpty(sorted_list->list);
}

sorted_list_iter_t SortedListFind(sorted_list_t* sorted_list,
                                sorted_list_iter_t from, sorted_list_iter_t to, 
                    	        const void* to_find)
                    	        
{
	sorted_element_t sort_element = {NULL,NULL};
	
	assert(NULL != sorted_list);
	assert(NULL != sorted_list->list);
	assert(NULL != from.inner_iter);
	assert(NULL != to.inner_iter);
	assert(TRUE == FromSameList(from,to));
	
	sort_element.param = (void*)to_find;
	sort_element.InsertionPointFunc = sorted_list->CmpFunc;
	
	from.inner_iter = DLLFind(from.inner_iter, to.inner_iter,CmpToFind, 
                     &sort_element); 
	return from;
}

sorted_list_iter_t SortedListFindIf(sorted_list_iter_t from, sorted_list_iter_t to, 
                            int (*IsMatchFunc)(const void* data, const void* param),
                            const void* param)
{
	assert(NULL != from.inner_iter);
	assert(NULL != to.inner_iter);
	assert(TRUE == FromSameList(from,to));
	assert(NULL != IsMatchFunc);
	
	from.inner_iter = DLLFind(from.inner_iter, to.inner_iter,IsMatchFunc, 
                     param); 
	return from;
}

int SortedListForEach(sorted_list_iter_t from, sorted_list_iter_t to, 
                 int (*ActionFunc)(void* list_data, void* param), 
                 void* param)
{
	assert(NULL != from.inner_iter);
	assert(NULL != to.inner_iter);
	assert(TRUE == FromSameList(from,to));
	assert(NULL != ActionFunc);
	
	return DLLForEach(from.inner_iter, to.inner_iter, ActionFunc, param);
}

void SortedListMerge(sorted_list_t* dest, sorted_list_t* src)
{
	node_t* from_src = NULL;
	node_t* to_src = NULL;
	node_t* where_dest = NULL;
	sorted_element_t sort_element = {NULL,NULL};
	
	assert(NULL != dest);
	assert(NULL != dest->list);	
	assert(NULL != src);
	assert(NULL != src->list);
	assert(NULL != src->CmpFunc);
	assert(NULL != dest->CmpFunc);
	assert(dest->CmpFunc == src->CmpFunc);
	
	to_src = DLLBegin(src->list);
	
	sort_element.InsertionPointFunc = dest->CmpFunc;
	
    while (FALSE == DLLIsSameIter(DLLBegin(src->list), DLLEnd(src->list)))
    {
        from_src = DLLBegin(src->list);
        where_dest = DLLBegin(dest->list);
        
    	sort_element.param = DLLGetData(from_src);
        where_dest = DLLFind(where_dest, DLLEnd(dest->list), 
                             CmpToInsert, &sort_element);
                             
    	sort_element.param = DLLGetData(where_dest);
        to_src = DLLFind(from_src, DLLEnd(src->list), 
                         CmpToInsert, &sort_element);
       
      	if (TRUE == DLLIsSameIter(from_src, to_src))
        {
            to_src = DLLNext(to_src);
        }
	
	 	DLLSplice(where_dest, from_src, to_src);	
    }
}


/*============================================================================*/
static sorted_list_iter_t CreateSortedListIter(dlist_iter_t iter,const sorted_list_t* sorted_list)
{
	sorted_list_iter_t new_iter = {0};
	
	assert(NULL != sorted_list);
	assert(NULL != iter);
	
	new_iter.inner_iter = iter;
	
	#ifndef NDEBUG
	new_iter.list = (sorted_list_t*)sorted_list;
	#else
	UNUSED(sorted_list);
	#endif/*NDEBUG*/
	
	return new_iter;
}
#ifndef NDEBUG
static int FromSameList(sorted_list_iter_t iter1, sorted_list_iter_t iter2)
{
	return iter1.list == iter2.list;
}
#endif/*NDEBUG*/
static int CmpToInsert(const void* a, const void* b)
{
	assert(NULL != a);
	assert(NULL != b);
	
	return 0 < ((sorted_element_t*)b)->InsertionPointFunc(a,((sorted_element_t*)b)->param);
}

static int CmpToFind(const void* a, const void* b)
{
	assert(NULL != a);
	assert(NULL != b);
	
	return 0 == ((sorted_element_t*)b)->InsertionPointFunc(a,((sorted_element_t*)b)->param);
}
