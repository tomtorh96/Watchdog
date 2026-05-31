 /**********************
 * test_sorted_list
 * Author: Tomer Harel
 * Reviewer: Yuval Chen
**********************/

#include <stddef.h> /*size_t*/
#include <stdio.h> /*printf*/

#include "sorted_list.h" /*typedef SortedListIter SortedList*/

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[1;33m"
#define COLOR_RESET   "\x1b[0m"

static int divedBy5(const void* a, const void* b);
static int SmallToBig(const void* a, const void* b);
static int IncreseByVal(void* a, void* b);

static void TestFindingNumber();
static void TestCreateSortedListInsertAndPOP();
static void TestCreateAndMerge();
static void TestFindElementDivedBy5();
static void TestForEachOnElements();

int main()
{
	TestCreateSortedListInsertAndPOP();
	TestCreateAndMerge();
	TestFindElementDivedBy5();
	TestForEachOnElements();
	TestFindingNumber();
	return 0;
}

static void TestCreateSortedListInsertAndPOP()
{
	sorted_list_t* list = NULL;
	int value = 5;
	int returned_value = 0;
	list = SortedListCreate(SmallToBig);
	if (NULL == list)
	{
		printf("TestCreateSortedListInsertAndPOP ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("Problem creating sorted list\n");
		return;
	}
	
	SortedListInsert(list,&value);
	
	if (1 != SortedListSize(list))
	{
		printf("TestCreateSortedListInsertAndPOP ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("expected %d got %lu\n", 1, SortedListSize(list));
		return;
	}
	returned_value = *(int*)SortedListPopFront(list);
	if (value != returned_value)
	{
		printf("TestCreateSortedListInsertAndPOP ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("expected %d got %d\n", value, returned_value);
		return;
	}
	
	if (FALSE == SortedListIsEmpty(list))
	{
		printf("TestCreateSortedListInsertAndPOP ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("list is not empty'n");
		return;
	}
	printf("TestCreateSortedListInsertAndPOP ");
	printf(COLOR_GREEN"PASSED\n"COLOR_RESET);
	SortedListDestroy(list);
}

static void TestCreateAndMerge()
{
	int arr1[] = {4,1,6,7};
	int arr2[] = {8,5,3,2};
	int arr_output[] = {1,2,3,4,5,6,7,8};
	sorted_list_t* list1 = NULL;
	sorted_list_t* list2 = NULL;
	size_t index = 0;
	size_t size = 0;
	sorted_list_iter_t iter;
	
	list1 = SortedListCreate(SmallToBig);
	if (NULL == list1)
	{
		printf("TestCreateAndMerge ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("Problem creating first sorted list\n");
		return;
	}
	
	for (; index < 4; ++index)
	{
		SortedListInsert(list1,&arr1[index]);
	}
	
	list2 = SortedListCreate(SmallToBig);
	if (NULL == list2)
	{
		printf("TestCreateAndMerge ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("Problem creating second sorted list\n");
		return;
	}
	
	for (index = 0; index < 4; ++index)
	{
		SortedListInsert(list2,&arr2[index]);
	}
	
	SortedListMerge(list1,list2);
	
	iter = SortedListBegin(list1);
	
	for (index = 0; index < 8; ++index)
	{
		if (arr_output[index] != *(int*)SortedListIterGetData(iter))
		{
			printf("TestCreateAndMerge ");
			printf(COLOR_RED"FAILED\n"COLOR_RESET);
			printf("in checking the array experted: %d got: %d\n",
			arr_output[index],*(int*)SortedListIterGetData(iter));
			return;
		}
		iter = SortedListNext(iter);
	}
	
	size = SortedListSize(list1);
	if (8 != size)
	{
			printf("TestCreateAndMerge ");
			printf(COLOR_RED"FAILED\n"COLOR_RESET);
			printf("in checking the array experted: %d got: %lu\n",8,size);
			return;
	}
	
	printf("TestCreateAndMerge ");
	printf(COLOR_GREEN"PASSED\n"COLOR_RESET);
	SortedListDestroy(list1);
	SortedListDestroy(list2);
}

static void TestFindElementDivedBy5()
{
	int arr[] = {1,2,3,4,5,6,7,8};
	sorted_list_t* list = NULL;

	size_t index = 0;
	sorted_list_iter_t iter;
	
	list = SortedListCreate(SmallToBig);
	if (NULL == list)
	{
		printf("TestFindElementDivedBy5 ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("Problem creating sorted list\n");
		return;
	}
	
	for (; index < 8; ++index)
	{
		SortedListInsert(list,&arr[index]);
	}
	
	iter = SortedListBegin(list);
	iter = SortedListFindIf(iter, SortedListEnd(list), divedBy5, &index);
	
	if (*(int*)SortedListIterGetData(iter) != arr[4])
	{
		printf("TestFindElementDivedBy5 ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("experted: %d got: %d\n",arr[4],
		*(int*)SortedListIterGetData(iter));
		return;
	}
	printf("TestFindElementDivedBy5 ");
	printf(COLOR_GREEN"PASSED\n"COLOR_RESET);
	SortedListDestroy(list);
}

static void TestForEachOnElements()
{
	int arr[] = {1,2,3,4,5,6,7,8};
	sorted_list_t* list = NULL;
	sorted_list_iter_t iter;
	size_t index = 0;
	int value = 10;
	int output = 0;
	
	list = SortedListCreate(SmallToBig);
	if (NULL == list)
	{
		printf("TestForEachOnElements ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("Problem creating sorted list\n");
		return;
	}
	
	for (; index < 8; ++index)
	{
		SortedListInsert(list,&arr[index]);
	}
	
	SortedListForEach(SortedListBegin(list), SortedListEnd(list), IncreseByVal,
	&value);
	
	iter = SortedListBegin(list);
	
	for (index = 0; index < 8; ++index)
	{
		if (arr[index] != *(int*)SortedListIterGetData(iter))
		{
			printf("TestForEachOnElements ");
			printf(COLOR_RED"FAILED\n"COLOR_RESET);
			printf("experted: %d got: %d\n",arr[index],
			*(int*)SortedListIterGetData(iter));
			return;
		}
		iter = SortedListNext(iter);
	}
	
	output = *(int*)SortedListPopBack(list);
	if (output != arr[7])
	{
			printf("TestForEachOnElements ");
			printf(COLOR_RED"FAILED\n"COLOR_RESET);
			printf("experted: %d got: %d\n",arr[7],
			output);
			return;
	}
	printf("TestForEachOnElements ");
	printf(COLOR_GREEN"PASSED\n"COLOR_RESET);
	SortedListDestroy(list);
}


static void TestFindingNumber()
{
	
	int arr[] = {56,89,10,2,7,300,69,33};
	sorted_list_t* list = NULL;
	sorted_list_iter_t iter;
	size_t index = 0;
	int value = 56;
	
	list = SortedListCreate(SmallToBig);
	if (NULL == list)
	{
		printf("TestFindingNumber ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("Problem creating sorted list\n");
		return;
	}
	
	for (; index < 8; ++index)
	{
		SortedListInsert(list,&arr[index]);
	}
	
	iter = SortedListFind(list, SortedListBegin(list), SortedListEnd(list),
	 &value);
	
	if (*(int*)SortedListIterGetData(iter) != value)
	{
			printf("TestFindingNumber ");
			printf(COLOR_RED"FAILED\n"COLOR_RESET);
			printf("experted: %d got: %d\n",value,
			*(int*)SortedListIterGetData(iter));
			return;
	}
	
	printf("TestFindingNumber ");
	printf(COLOR_GREEN"PASSED\n"COLOR_RESET);
	SortedListDestroy(list);
}

static int SmallToBig(const void* a, const void* b)
{
	return *(int*)a - *(int*)b; 
}

static int divedBy5(const void* a, const void* b)
{
	(void)b;
	
	return *(int*)a % 5 == 0;
}

static int IncreseByVal(void* a, void* b)
{
	*(int*)a += *(int*)b;
	
	return 0;
}
