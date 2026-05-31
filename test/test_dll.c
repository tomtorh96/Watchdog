/**********************
 * dll
 * Author: Tomer Harel
 * Reviewer: Shahar Lasri 
**********************/

#include <stdio.h>/*printf*/

#include "dll.h" /*typedef, API*/

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[1;33m"
#define COLOR_RESET   "\x1b[0m"

static void TestInsertingAndCountElements();
static void TestCountEmptyList();
static void TestSplicingLists();
static void TestMultiFind();

static int IsEqualNum(const void* data, const void* param);
int main()
{
	TestInsertingAndCountElements();
	TestCountEmptyList();
	TestSplicingLists();
	TestMultiFind();
	return 0;
}

static void TestInsertingAndCountElements()
{
	size_t arr[] = {1,2,3,4,5};					
	dll_t* dll = NULL;
	size_t size = 0;
	size_t index = 0;
	
	dll = DLLCreate();
	if (NULL == dll)
	{
		printf("TestInsertingAndCountElements ");
		printf("couldn't create dll");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		return;
	}
	
	for (; index < 5; ++index)
	{	
		if (DLLEnd(dll) == DLLPushFront(dll, &arr[index]))
		{
			printf("TestInsertingAndCountElements ");
			printf("problem inserting element %lu",arr[index]);
			printf(COLOR_RED"FAILED\n"COLOR_RESET);
			DLLDestroy(dll);
			return;
		}
		
	}
	
	size = DLLCount(dll);
	if (size != index)
	{
		printf("TestInsertingAndCountElements ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("expected: %lu got: %lu\n",index,size);
		return;
	}
	
	printf("TestInsertingAndCountElements ");
	printf(COLOR_GREEN"PASSED\n"COLOR_RESET);
	DLLDestroy(dll);
}

static void TestCountEmptyList()
{
	dll_t* dll = NULL;
	size_t size = 0;
	int value = 5;
	dlist_iter_t iter = NULL;
	
	dll = DLLCreate();
	if (NULL == dll)
	{
		printf("TestCountEmptyList ");
		printf("couldn't create dll");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		return;
	}
	iter = DLLBegin(dll);
	iter = DLLInsert(dll,iter,&value);
	DLLRemove(iter);
	
	size = DLLCount(dll);
	if (0UL != size && 1 == DLLIsEmpty(dll))
	{
		printf("TestCountEmptyList ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("expected: %d got: %lu\n",0,size);
		return;
	}
	
	printf("TestCountEmptyList ");
	printf(COLOR_GREEN"PASSED\n"COLOR_RESET);
	DLLDestroy(dll);
}

static void TestSplicingLists()
{
	int arr[] = {1,2,3,4,5,6,7,8,9,10};
	dll_t* dll1 = NULL;
	dll_t* dll2 = NULL;
	size_t size = 0;
	size_t index = 0;
	dlist_iter_t iter_form = NULL;
	dlist_iter_t iter_to = NULL;
	dlist_iter_t iter_where = NULL;
	
	dll1 = DLLCreate();
	if (NULL == dll1)
	{
		printf("TestSplicingLists ");
		printf("couldn't create dll1");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		return;
	}
	
	for (; index < 5; ++index)
	{	
		DLLPushFront(dll1, &arr[index]);
	}
	
	iter_where = DLLBegin(dll1);
	iter_where = DLLNext(iter_where);
	
	dll2 = DLLCreate();
	if (NULL == dll2)
	{
		printf("TestSplicingLists ");
		printf("couldn't create dll2");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		return;
	}
	
	for (; index < 10; ++index)
	{	
		DLLPushFront(dll2, &arr[index]);
	}
	
	iter_form = DLLBegin(dll2);
	iter_form = DLLNext(iter_form);
	iter_to = DLLNext(iter_form);
	iter_to = DLLNext(iter_to);
	
	DLLSplice(iter_where, iter_form, iter_to);
	
	size = DLLCount(dll1);
	if (7 != size)
	{
		printf("TestSplicingLists ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("in dll1 expected: %u got: %lu\n",7,size);
		return;
	}
	
	size = DLLCount(dll2);
	if (3 != size)
	{
		printf("TestSplicingLists ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("in dll2 expected: %u got: %lu\n",3,size);
		return;
	}
	
	printf("TestSplicingLists ");
	printf(COLOR_GREEN"PASSED\n"COLOR_RESET);
	DLLDestroy(dll1);
	DLLDestroy(dll2);
}

static void TestMultiFind()
{
	int arr[] = {1,2,3,4,3,6,7,3,9,3};
	dll_t* dll1 = NULL;
	dll_t* dll2 = NULL;
	size_t size = 0;
	size_t index = 0;
	dlist_iter_t iter_form = NULL;
	dlist_iter_t iter_to = NULL;
	
	dll1 = DLLCreate();
	if (NULL == dll1)
	{
		printf("TestMultiFind ");
		printf("couldn't create dll1");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		return;
	}
	
	dll2 = DLLCreate();
	if (NULL == dll2)
	{
		printf("TestMultiFind ");
		printf("couldn't create dll2");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		return;
	}
	
	for (; index < 10; ++index)
	{	
		DLLPushFront(dll1, &arr[index]);
	}
	
	iter_form = DLLBegin(dll1);
	iter_to = DLLEnd(dll1);
	
	if (0 != DLLMultiFind(iter_form,iter_to,IsEqualNum,&arr[2],dll2))
	{
		printf("TestMultiFind ");
		printf("problem with multifind insert");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		return;
	}
	
	size = DLLCount(dll2);
	if (4 != size)
	{
		printf("TestMultiFind ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("in dll2 expected: %u got: %lu\n",4,size);
		return;
	}
	
	if (arr[2] != *(int*)DLLGetData(DLLBegin(dll2)))
	{
		printf("TestMultiFind ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("in dll2 expected: %d got: %d\n",arr[2],*(int*)DLLGetData(DLLBegin(dll2)));
		return;
	}
	
	printf("TestMultiFind ");
	printf(COLOR_GREEN"PASSED\n"COLOR_RESET);
	DLLDestroy(dll1);
	DLLDestroy(dll2);
}


static int IsEqualNum(const void* data, const void* param)
{
	return *(int*)data == *(int*)param;
}
