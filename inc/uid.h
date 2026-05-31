/**********************
 * uid.h
 * Author: Tomer Harel
 * Reviewer: Shahar Lasri 
**********************/

#ifndef __ILRD_UID_H__
#define __ILRD_UID_H__

#include <time.h> /*time_t*/
#include <sys/types.h> /*pid_t*/

typedef struct UID
{
    time_t timestamp;
    size_t counter;
    pid_t pid;
    unsigned char ip_address[14];
} ilrd_uid_t;

extern const ilrd_uid_t UIDBadUID;

/**
 * Description: Creates an UID.
 *				
 * Arguments:   -
 *              
 * Return:      Success - Newly created ilrd_uid_t
 *              Failure - UIDBadUID
 * 
 * Complexity:  Time: O(1), 
 *              Space: O(1)
**/ 
ilrd_uid_t UIDCreate(void);

/**
 * Description: Compares two UIDs and returns if they are the same.
 *				
 * Arguments:   uid1 - First unique identifier to compare 
 *              uid2 - Second unique identifier to compare 
 * 
 * Return:      True  - (1), 
 *              False - (0)
 * 
 * Complexity:  Time: O(1), 
 *              Space: O(1)
**/ 
int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2);

#endif /* __ILRD_UID_H__ */
