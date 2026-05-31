 /**********************
 * test_pq.c
 * Author: Tomer Harel
 * Reviewer: maxim shteingard
**********************/

#include <stdio.h>/*printf*/

#include "pq.h" /*pq_t*/

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[1;33m"
#define COLOR_RESET   "\x1b[0m"

#define FALSE (0)

static int SmallToBig(const void* a, const void* b);
static int IsSame(const void* a, const void* b);

static void TestCreatePQInsertAndPop();
static void TestNumberInPQ();
static void TestPeekAndClear();
int main()
{
	TestCreatePQInsertAndPop();
	TestNumberInPQ();
	TestPeekAndClear();
	return 0;
}

static void TestCreatePQInsertAndPop()
{
	pq_t* list = NULL;
	int value = 5;
	int returned_value = 0;
	
	list = PQCreate(SmallToBig);
	if (NULL == list)
	{
		printf("TestCreatePQInsertAndPop ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("Problem creating sorted list\n");
		PQDestroy(list);
		return;
	}
	
	PQEnqueue(list,&value);
	
	if (1 != PQSize(list))
	{
		printf("TestCreatePQInsertAndPop ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("expected %d got %lu\n", 1, PQSize(list));
		PQDestroy(list);
		return;
	}
	returned_value = *(int*)PQDequeue(list);
	if (value != returned_value)
	{
		printf("TestCreatePQInsertAndPop ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("expected %d got %d\n", value, returned_value);
		PQDestroy(list);
		return;
	}
	
	if (FALSE == PQIsEmpty(list))
	{
		printf("TestCreatePQInsertAndPop ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("list is not empty'n");
		PQDestroy(list);
		return;
	}
	
	printf("TestCreatePQInsertAndPop ");
	printf(COLOR_GREEN"PASSED\n"COLOR_RESET);
	
	PQDestroy(list);
}

static void TestNumberInPQ()
{
	
	int arr[] = {56,89,10,2,7,300,69,33};
	pq_t* list = NULL;
	size_t index = 0;
	int value = 56;
	int found_value = 0;
	size_t size = 8;
	size_t output_size = 0;
	list = PQCreate(SmallToBig);
	if (NULL == list)
	{
		printf("TestNumberInPQ ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("Problem creating sorted list\n");
		PQDestroy(list);
		return;
	}
	
	for (; index < size; ++index)
	{
		PQEnqueue(list,&arr[index]);
	}
	
	found_value = *(int*)PQErase(list,IsSame,&value);
	
	if (found_value != value)
	{
			printf("TestNumberInPQ ");
			printf(COLOR_RED"FAILED\n"COLOR_RESET);
			printf("experted: %d got: %d\n",value,found_value);
			PQDestroy(list);
			return;
	}
	
	output_size = PQSize(list);
	if (size -1 != output_size)
	{
			printf("TestNumberInPQ ");
			printf(COLOR_RED"FAILED\n"COLOR_RESET);
			printf("experted: %lu got: %lu\n",size -1,output_size);
			PQDestroy(list);
			return;
	}
	
	printf("TestNumberInPQ ");
	printf(COLOR_GREEN"PASSED\n"COLOR_RESET);
	PQDestroy(list);
}

static void TestPeekAndClear()
{
	int arr[] = {56,89,10,2,7,300,69,33};
	pq_t* list = NULL;
	size_t index = 0;
	int value = 300;
	size_t size = 8;
	list = PQCreate(SmallToBig);
	if (NULL == list)
	{
		printf("TestPeekAndClear ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("Problem creating sorted list\n");
		PQDestroy(list);
		return;
	}
	
	for (; index < size; ++index)
	{
		PQEnqueue(list,&arr[index]);
	}
		
	if (value != *(int*)PQPeek(list))
	{
			printf("TestPeekAndClear ");
			printf(COLOR_RED"FAILED\n"COLOR_RESET);
			printf("experted: %d got: %d\n",value,*(int*)PQPeek(list));
			PQDestroy(list);
			return;
	}
	
	PQClear(list);
	
	if (FALSE == PQIsEmpty(list))
	{
		printf("TestPeekAndClear ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("list is not empty'n");
		PQDestroy(list);
		return;
	}
	
	printf("TestPeekAndClear ");
	printf(COLOR_GREEN"PASSED\n"COLOR_RESET);
	PQDestroy(list);
}

static int SmallToBig(const void* a, const void* b)
{
	return *(int*)a - *(int*)b; 
}

static int IsSame(const void* a, const void* b)
{
	return !SmallToBig(a,b);
}
