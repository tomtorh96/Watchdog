/**********************
 * test_user.c
 * Author: Tomer Harel
 * Reviewer: Name
 **********************/

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include "watchdog.h"

#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[1;33m"
#define COLOR_RESET "\x1b[0m"

int main(int argc, char* argv[])
{
	size_t tolerance = 1;
	size_t tolerance_interval = 4;
	unsigned int time = 60;
	
	WDStart(tolerance, tolerance_interval, argc, argv);

	printf("doing work\n");
	while (0 < time)
	{
		time = sleep(time);
	}

	printf("finished work\n");

	WDStop();

	return 0;
}