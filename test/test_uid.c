 /**********************
 * test_uid.c
 * Author: Tomer Harel
 * Reviewer: Shahar Lasri 
**********************/

#include <stdio.h>/*printf*/

#include "uid.h" /*ilrd_uid_t*/

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[1;33m"
#define COLOR_RESET   "\x1b[0m"

static void TestCreateNewUID();
static void TestCheckIfUIDISSame();

int main()
{
	TestCreateNewUID();
	TestCheckIfUIDISSame();
	return 0;
}

static void TestCreateNewUID()
{
	ilrd_uid_t uid = UIDCreate();
	
	printf("timestemp: %ld counter: %lu pid: %u ip: %s\n",
	uid.timestamp,uid.counter,uid.pid,uid.ip_address);
	
	printf("TestCreateNewUID ");
	printf(COLOR_GREEN"PASSED\n"COLOR_RESET);
}

static void TestCheckIfUIDISSame()
{
	ilrd_uid_t uid1 = UIDCreate();
	ilrd_uid_t uid2 = UIDCreate();
	
	if (0 == UIDIsSame(uid1,uid1))
	{
		printf("TestCheckIfUIDISSame ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("same UID got as diffrent\n");
		return;
	}
	
	if (1 == UIDIsSame(uid1,uid2))
	{
		printf("TestCheckIfUIDISSame ");
		printf(COLOR_RED"FAILED\n"COLOR_RESET);
		printf("diffrent UID got as same\n");
		return;
	}
	
	printf("TestCheckIfUIDISSame ");
	printf(COLOR_GREEN"PASSED\n"COLOR_RESET);
}
