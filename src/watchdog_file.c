
#define _POSIX_C_SOURCE 200809L

#include <stddef.h>	   /*size_t*/
#include <stdio.h>	   /*BUFSIZ*/
#include <stdlib.h>	   /*strtol*/
#include <assert.h>	   /*assert*/
#include <pthread.h>   /*pthred_t*/
#include <semaphore.h> /*sem_t*/
#include <signal.h>	   /*SIGUSR1,SIGUSR2*/
#include <string.h>	   /*strcpy*/
#include <sys/wait.h>  /*waitpid*/
#include <unistd.h>	   /*STDOUT_FILENO*/

#include "scheduler.h" /*typedef sched_t and API*/
#include "watchdog_ex.h"	   /*WDWatch, WDRevive api*/
#include "watchdog.h"  /*WDstart, WDStop api*/

#define UNUSE(x) ((void)(x))
#define STR_LEN (6)
#define WD_EXTRA_ARGS (3)
#define TOLERANCE_INDEX (2)
#define INTERVAL_INDEX (1)
#define FAILURE (1)
#define TRUE (1)
#define FALSE (0)
#define SEMAPHORE_NAME ("shared_sem")

/************************************************ Functions Declarations *****/
static void ReviveUser(void* param);

static char user_file_name[BUFSIZ] = {0};

int main(int argc, char* argv[])
{
	size_t tolerance = 0;
	size_t tolerance_interval = 0;
	wd_t wd_data = {0};

	wd_data.sem_wd_handshake = sem_open(SEMAPHORE_NAME, 0);
	tolerance = strtol(argv[argc - TOLERANCE_INDEX], NULL, 10);
	tolerance_interval = strtol(argv[argc - INTERVAL_INDEX], NULL, 10);

	strcpy(user_file_name, argv[0]);

	wd_data.tolerance = tolerance;
	wd_data.interval = tolerance_interval;
	wd_data.sent_argv = argv;
	wd_data.is_wd = TRUE;
	argv[argc - WD_EXTRA_ARGS] = NULL;

	if (FAILURE == InitSignalsActions())
	{
		return WD_ERROR;
	}

	wd_data.revive_func = ReviveUser;

	WDWatch(&wd_data);
	sem_close(wd_data.sem_wd_handshake);

	return WD_SUCCESS;
}

static void ReviveUser(void* param)
{
	wd_t data = *(wd_t*)param;

	assert(NULL != param);

	SchedDestroy(data.sched);

	execv(user_file_name, data.sent_argv);
	exit(1);
}
