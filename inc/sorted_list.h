 /**********************
 * sorted_list.h
 * Author: Tomer Harel
 * Reviewer: Yuval Chen
**********************/

#ifndef __ILRD_SORTED_LIST_H__
#define __ILRD_SORTED_LIST_H__

#include "dll.h" /*typedef dll, dll API*/

typedef struct SortedList sorted_list_t;
typedef struct SortedListIter sorted_list_iter_t;

struct SortedListIter
{
	dlist_iter_t inner_iter;
#ifndef NDEBUG
	sorted_list_t* list;
#endif /*NDEBUG*/
};

/**
 * Description:  Creates a Sorted list.
 *
 * Arguments:    CmpFunc - Pointer to a comparison function, must not be NULL.
 *
 * Return:       Success - Pointer to a newly allocated sorted linked list.
 *               Failure - NULL.
 *
 * Complexity:   Time:  O(1)
 *               Space: O(1)
 */
sorted_list_t* SortedListCreate(int (*CmpFunc)(const void* element1, const void* element2));

/**
 * Description:  Destroys a sorted list.
 *
 * Arguments:    sorted_list - Pointer to a sorted list, must not be NULL.
 *
 * Return:       -
 * 
 * Complexity:   Time:  O(n)
 *               Space: O(1)
 */
void SortedListDestroy(sorted_list_t* sorted_list);

/**
 * Description:  Returns an iterator to the beggining of the sorted list.
 *
 * Arguments:    sorted_list - valid pointer to a sorted list, must not be NULL.
 *
 * Return:       Iterator representing the beggining of the sorted list.
 * 
 * Complexity:   Time:  O(1)
 *               Space: O(1)
 */
sorted_list_iter_t SortedListBegin(const sorted_list_t* sorted_list);

 /**
 * Description:  Returns an iterator to the end of the sorted list.
 *
 * Arguments:    sorted_list - valid pointer to a sorted list, must not be NULL.
 *
 * Return:       Iterator representing the end of the list. 
 *               End of the list is an element,
 *               after the last user inserted element.
 * 
 * Complexity:   Time:  O(1)
 *               Space: O(1)
 */
sorted_list_iter_t SortedListEnd(const sorted_list_t* sorted_list);

 /**
 * Description:  Advances the iterator to the next element in the list.
 *
 * Arguments:    current - Iterator of the current element of the sorted list. 
 *					       Must be a valid iterator, and must not be SortedListEnd.
 *
 * Return:       Iterator of the element following the current element.
 * 
 * Complexity:   Time:  O(1)
 *               Space: O(1)
 */
sorted_list_iter_t SortedListNext(sorted_list_iter_t current);

 /**
 * Description:  Returns an iterator to the previous element in the list.
 *
 * Arguments:    current - iterator of the current element of the sorted list.
 *					       Must be a valid iterator, and not be SortedListBegin.
 *
 * Return:       Iterator of the list preceding the current element.
 * 
 * Complexity:   Time:  O(1)
 *               Space: O(1)
 */
sorted_list_iter_t SortedListPrev(sorted_list_iter_t current);

/**
 * Description:  Checks if two iterators point to the same element in the list.
 *
 * Arguments:    iter1 - First iterator to compare, must be a valid iterator.
 *               iter2 - Second iterator to compare, must be a valid iterator.
 *               
 * Return:       TRUE  - (1) - if both iterators point to the same location.
 *               FALSE - (0) - otherwise.
 *
 * Complexity:   Time:  O(1)
 *			     Space: O(1)
**/
int SortedListIterIsEqual(sorted_list_iter_t iter1, sorted_list_iter_t iter2);

 /**
 * Description:  Gets the data from the element pointed to by the iterator.
 *
 * Arguments:    iter - Iterator of the list. Must be a valid iterator
 *                      and must not be SortedListEnd.
 *
 * Return:       Pointer to the user data stored in the element.
 *
 * Complexity:   Time:  O(1)
 *               Space: O(1)
 */
void* SortedListIterGetData(sorted_list_iter_t iter);

 /**
 * Description:  Inserts a new element into the sorted list while maintaining 
 *               the sorting order based on the list's comparison function.
 *
 * Arguments:    sorted_list - Pointer to the sorted list. Must not be NULL.
 *               data        - Pointer to data to be inserted.
 *               
 * Return:       On Success - Iterator pointing to the newly inserted element.
 *			     On Failure - returns SortedListEnd.
 *
 * Complexity:   Time:  O(n)
 *			     Space: O(1)
**/
sorted_list_iter_t SortedListInsert(sorted_list_t* sorted_list, void* data); 

 /**
 * Description:  Removes the element pointed to by the iterator from the list.
 *               
 * Arguments:    iter - Iterator of the element to be removed.
 *                      Must be a valid iterator and must not be SortedListEnd.
 *               
 * Return:       Iterator pointing to the element following 
 *               the one that was removed.
 *
 * Complexity:   Time:  O(1)
 *			     Space: O(1)
**/
sorted_list_iter_t SortedListRemove(sorted_list_iter_t iter);

 /**
 * Description:  Removes the first element from the sorted list 
 *               and returns its data.
 *               
 * Arguments:    sorted_list - Pointer to the sorted list. 
 *                             Must not be NULL and must not be empty.
 *               
 * Return:       Pointer to the data of the removed element.
 *
 * Complexity:   Time:  O(1)
 *			     Space: O(1)
**/
void* SortedListPopFront(sorted_list_t* sorted_list);

 /**
 * Description:  Removes the last element from the sorted list 
 *               and returns its data.
 *               
 * Arguments:    sorted_list - Pointer to the sorted list. 
 *                             Must not be NULL and must not be empty.
 *               
 * Return:       Pointer to the data of the removed element.
 *
 * Complexity:   Time:  O(1)
 *			     Space: O(1)
**/
void* SortedListPopBack(sorted_list_t* sorted_list);

/**
 * Description: Counts the number of elements currently in the sorted list.
 *
 * Arguments:   sorted_list - Valid pointer to a sorted list.
 *                            Must not be NULL.
 *               
 * Return:      The total number of user-inserted elements in the list.
 *
 * Complexity:  time:  O(n)
 *			    space: O(1)
**/
size_t SortedListSize(const sorted_list_t* sorted_list);

/**
 * Description: Checks whether the sorted list contains any elements.
 *
 * Arguments:   sorted_list - Pointer to sorted list, must not be NULL.
 *               
 * Return:      TRUE  - (1) - If list is empty returns.
 *              FALSE - (0) - otherwise.
 *
 * Complexity:  time:  O(1)
 *			    space: O(1)
**/
int SortedListIsEmpty(const sorted_list_t* sorted_list);

/**
 * Description: Searches for a specific element in the range [from, to) using 
 *              the list's internal comparison function.
 * 
 * Arguments:   sorted_list    - Pointer to the sorted list to be searched.
 *              from           - Iterator pointing to where the search starts (inclusive). 
 *                               A valid iterator, comes before 'to' iterator,
 *              to             - Iterator pointing to where the search ends (exclusive). 
 *                               A valid iterator, comes after 'from' iterator,
 *              to_find        - Pointer to the data to be searched for.
 *               
 * Return:      On Success     - Iterator pointing to the first element that matches 'to_find'.
 *              ON Failure     - The 'to' iterator (if no matching element is found).
 *
 * Complexity:  time:  O(n)
 *			    space: O(1)
**/
sorted_list_iter_t SortedListFind(sorted_list_t* sorted_list,
                                  sorted_list_iter_t from, 
                                  sorted_list_iter_t to, 
                    	          const void* to_find);

/**
 * Description: Searches for the first element in the range [from, to) that 
 *              satisfies a given condition.
 * 
 * Arguments:   from           - Iterator pointing to where the search starts (inclusive).
 *                               Must be a valid iterator, comes before 'to' iterator.				         
 *              to             - Iterator pointing to where the search ends (exclusive). 
 *                               Must be a valid iterator, comes after 'from' iterator.
 *              IsMatchFunc    - Function pointer to a valid Matching function.
 *                               Return 1 - if the element matches, otherwise return 0.
 *              param          - Additional parameter to be passed to the match function.            
 * Return:      On Success     - Iterator pointing to the first matching element found.
 *              On Failure     - The 'to' iterator (if no matching element is found).
 *
 * Complexity:  time:  O(n)
 *			    space: O(1)
**/
sorted_list_iter_t SortedListFindIf(sorted_list_iter_t from, sorted_list_iter_t to, 
                            int (*IsMatchFunc)(const void* data, const void* param),
                            const void* param); 

/**
 * Description: Applies a given action function to each element in the range [from, to).
 *
 * Arguments:   from       - Iterator pointing to where the action starts (inclusive).
 *                           Must be a valid iterator, which comes before the 'to' iterator.
 *              to         - Iterator pointing to where the action ends (exclusive). 
 *                           Must be a valid iterator, which comes after the 'from' iterator.
 *              ActionFunc - Function pointer to the action to be performed. Must not be NULL.
 *              param      - Parameter to be passed to the action function.
 *               
 * Return:      On Success - (0)
 *              On Failure - Non-zero status returned by the action function when failed.
 *
 * Complexity:  time:  O(n)
 *			    space: O(1)
**/
int SortedListForEach(sorted_list_iter_t from, sorted_list_iter_t to, 
                 int (*ActionFunc)(void* list_data, void* param), 
                 void* param);

/**
 * Description: Merges all elements from the 'src' list into the 'dest' list.
 * 
 * Arguments:   dest   - Pointer to the sorted list where elements will be added.
 *                       Diffrent fron src.
 *                       After the merge dest remains sorted.
 *                       
 *              src    - Pointer to the sorted list to be merged. 
 *                       Must be a valid sorted list, and must not be empty.
 *                       After the merge src will be empty.
 *              Assumes 'dest' and 'src' are using the same comparison logic. 
 * Return:      -
 *
 * Complexity:  time:  O(n + m) where n and m are the lengths of the two lists.
 *			    space: O(1)
**/
void SortedListMerge(sorted_list_t* dest, sorted_list_t* src); 

#endif /* __ILRD_SORTED_LIST_H__ */

