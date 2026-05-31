/**********************
 * dll.h
 * Author: Tomer Harel
 * Reviewer: Shahar Lasri 
**********************/

#ifndef __ILRD_DLL_H__
#define __ILRD_DLL_H__

#define TRUE (1)
#define FALSE (0)

typedef struct DLL dll_t; 
typedef struct DLLNode* dlist_iter_t;

typedef enum
{
    ERROR = -1,
    SUCCESS = 0
} status_t;

/**
 * Description:  Creates a DLL.
 *
 * Arguments:    -
 *
 * Return:       Success - Pointer to a newly allocated doubly linked list,
 *               Failure - NULL.
 *
 * Complexity:   Time:  O(1)
 *               Space: O(1)
 */
dll_t* DLLCreate(void);

/**
 * Description:  Destroys a DLL.
 *
 * Arguments:    list - Pointer to a DLL.
 *
 * Return:       -
 * 
 * Complexity:   Time:  O(n)
 *               Space: O(1)
 */
void DLLDestroy(dll_t* list);

/**
 * Description:  Get an iterator at the first element of a given DLL.
 *
 * Arguments:    list - valid pointer to a DLL.
 *
 * Return:       Iterator of the list at the first element of the DLL.
 * 
 * Complexity:   Time:  O(1)
 *               Space: O(1)
 */
dlist_iter_t DLLBegin(const dll_t* list);

 /**
 * Description:  Get an iterator at the end element of a given DLL.
 *
 * Arguments:    list - valid pointer to a DLL.
 *
 * Return:       The element at the end of the DLL after the last user inserted element.
 * 
 * Complexity:   Time:  O(1)
 *               Space: O(1)
 */
dlist_iter_t DLLEnd(const dll_t* list);

 /**
 * Description:  Get an iterator and advances it to the next element.
 *
 * Arguments:    current - iterator of the current element of the DLL, 
 *					       valid iterator that is diffrent from the 
 * 						   result of DLLEnd.
 *
 * Return:       Iterator of the list at the next element in the DLL.
 * 
 * Complexity:   Time:  O(1)
 *               Space: O(1)
 */
dlist_iter_t DLLNext(dlist_iter_t current);

 /**
 * Description:  Get an iterator and retreats it to the previous element.
 *
 * Arguments:    current - iterator of the current element of the DLL, 
 *					       valid iterator that is diffrent from the 
 * 						   result of DLLBegin.
 *
 * Return:       Iterator of the list at the previous element in the DLL.
 * 
 * Complexity:   Time:  O(1)
 *               Space: O(1)
 */
dlist_iter_t DLLPrev(dlist_iter_t current);

/**
 * Description:  Compares two iterators and returns if they are equal.
 *
 * Arguments:    iter1 - first iterator to compare, a valid iterator.
 *               iter2 - second iterator to compare, a valid iterator.
 *               
 * Return:       TRUE  - (1)
 *               FALSE - (0)
 *
 * Complexity:   Time:  O(1)
 *			     Space: O(1)
**/
int DLLIsSameIter(dlist_iter_t iter1, dlist_iter_t iter2);

 /**
 * Description:  Gets the data pointed by the iterator.
 *
 * Arguments:    iter - iterator of the list, a valid iterator
 *                      which is not equal to DLLEnd.
 *
 * Return:       The data pointed by the iterator.
 *
 * Complexity:   Time:  O(1)
 *               Space: O(1)
 */
void* DLLGetData(dlist_iter_t iter);

 /**
 * Description:  Adds new element before the current element pointed by the iterator.
 *
 * Arguments:    list -  A valid pointer to a DLL.
 *               where - A valid iterator of the list.
 *               data -  Pointer to data to be inserted.
 *               
 * Return:       Success - Iterator of the list, pointing to the inserted element.
 *			     Failure - Itertor equal to DLLEnd.
 *
 * Complexity:   Time: O(1)
 *			     Space: O(1)
**/
dlist_iter_t DLLInsert(dll_t* list, dlist_iter_t where, void* data); 

/**
 * Description:  Removes the element pointed by the iterator. 
 *               Will not work on an empty list.
 *
 * Arguments:    iter - iterator to the element to be removed.
 *						valid iterator that is diffrent from 
 *						DLLEnd.
 *
 * Return:       Iterator that points to the element after the removed element position.
 *
 * Complexity:   Time:  O(1)
 *               Space: O(1)
 */
dlist_iter_t DLLRemove(dlist_iter_t iter);

 /**
 * Description:  Inserts data element at the beginning of the DLL
 *               
 * Arguments:    list -  Pointer to a DLL, a valid pointer  
 *               data -  Pointer to the data
 *               
 * Return:       Success - Iterator of the list, pointing to the inserted element.
 *			     Failure - Itertor equal to DLLEnd.
 * 
 * Complexity:   Time: O(1)
 *			     Space: O(1)
**/
dlist_iter_t DLLPushFront(dll_t* list, void* data);

 
 /**
 * Description:  Inserts data element at the end of the DLL
 *               
 * Arguments:    list -  Pointer to a DLL, a valid pointer  
 *               data -  Pointer to the data
 *               
 * Return:       Success - Iterator of the list, pointing to the inserted element.
 *			     Failure - Itertor equal to DLLEnd.
 * 
 * Complexity:   Time: O(1)
 *			     Space: O(1)
**/
dlist_iter_t DLLPushBack(dll_t* list, void* data);

 /**
 * Description:  Removes the element at the beginning of list.
 *               
 * Arguments:    list - Pointer to a valid DLL, DLL not empty
 *               
 * Return:       Returns the removed element's data
 *
 * Complexity:   Time: O(1)
 *			     Space: O(1)
**/
void* DLLPopFront(dll_t* list);

 /**
 * Description:  Removes the element at the end of list.
 *               Will not work on an empty list.
 *               
 * Arguments:    list - Pointer to a valid DLL, DLL not empty
 *               
 * Return:       Returns the removed element's data
 *
 * Complexity:   Time: O(1)
 *			     Space: O(1)
**/
void* DLLPopBack(dll_t* list);

/**
 * Description: Counts the number of elements in list.
 *
 * Arguments:   list - valid pointer to a DLL
 *               
 * Return:      The number of elements in the list
 *
 * Complexity:  time:  O(n)
 *			    space: O(1)
**/
size_t DLLCount(const dll_t* list);

/**
 * Description: Checks if list is empty.
 *
 * Arguments:   list - Pointer to DLL list, must not be NULL.
 *               
 * Return:      If list is empty returns TRUE - (1), 
 *              If list isn't empty returns FALSE - (0).
 *
 * Complexity:  time:  O(1)
 *			    space: O(1)
**/
int DLLIsEmpty(const dll_t* list);

/**
 * Description: Finds the first element, from a given start position (from)
 *              to a given end position (to), that matchs given data (param) 
 *              with given IsMatch function.
 * 
 * Arguments:   from -    Iterator pointing to where the function starts 
 *                        the search, a valid iterator, comes before 'to' iterator.
 *              to   -    Iterator pointing to where the function stops 
 *                        the search, a valid iterator, comes after 'from' iterator.
 *              IsMatch - Match function that checks match to DLL data element.
 *                        a valid function address.
 *                        Arguments: list_data - data element of DLL.
 *                                   param     - input of matcher function
 *                                   Returns:   TRUE - (1) or FALSE - (0).
 *              param   - Input of matcher function
 *               
 * Return:      When found     - Iterator of the list pointing to matched data element
 *              when not found - Iterator of 'to' position.
 *
 * Complexity:  time:  O(n)
 *			    space: O(1)
**/
dlist_iter_t DLLFind(dlist_iter_t from, dlist_iter_t to, 
                     int (*IsMatch)(const void* list_data, const void* param), 
                     const void* param); 

/**
 * Description: Search for data in the DLL, and save all the founded elements
 *              in dest list.
 * 
 * Arguments:   from    - Iterator pointing to where the function starts 
 *                        the search, a valid iterator, comes before 'to' iterator.
 *              to      - Iterator pointing to where the function stops 
 *                        the search, a valid iterator, comes after 'from' iterator.
 *              IsMatch - Match function that checks match to DLL data element.
 *                        a valid function address.
 *                        Arguments:   list_data - data element of DLL.
 *                                     param     - input of matcher function
 *                        Returns:   TRUE - (1) or FALSE - (0).
 *              param   - Input of matcher function
 *              dest_list - a valid list to store the found elements.
 * 
 * Return:      SUCCESS - (0) or ERROR - (-1)
 *
 * Complexity:  time:  O(n)
 *			    space: O(m)
**/
int DLLMultiFind(dlist_iter_t from, dlist_iter_t to,
                 int (*IsMatch)(const void* list_data, const void* param),
                 const void* param, 
                 dll_t* dest_list); 

/**
 * Description: operate an action on a param for each list-element from 'from' to 'to'
 *
 * Arguments:   from         Iterator addressing start element (including),
 *                           a valid iterator, which comes before the 'to' iterator
 *              to           Iterator addressing of last element (excluding), 
 *                           a valid iterator, which comes after the 'from' iterator
 *              ActionFunc   Function pointer to a valid action function
 *              param        a paramer to operate the action on
 *               
 * Return:      Success - (0)
 *              Failure - non-zero status returned by the action function when failed
 *
 * Complexity:  time:  O(n)
 *			    space: O(1)
**/
int DLLForEach(dlist_iter_t from, dlist_iter_t to, 
               int (*ActionFunc)(void* list_data, void* param), 
               void* param);

/**
 * Description: Splices a range of elements stating from 'form' up to 'to' into
 *              a specific position (where).
 * 
 *              This operation removes the elements starting from 
 *              'from' (inclusive) up to 'to' (excluding) from their original 
 *              position and inserts them before the 'where' iterator.  
 *
 * Arguments:   where   - Iterator indicating the insertion point in the 
 *                        destination, a valid iterator
 *              from    - Iterator indicating the starting point to move.
 *                        Valid iterator, which comes before 'to' iterator
 *              to      - Iterator indicating the end point to move.
 *                        Valid iterator,  which comes after 'from' iterator
 *               
 * Return:      Iterator to last element which was inserted.
 *
 * Complexity:  time:  O(1)
 *			    space: O(1)
**/
dlist_iter_t DLLSplice(dlist_iter_t where, dlist_iter_t from, dlist_iter_t to); 

#endif /* __ILRD_DLL_H__ */

