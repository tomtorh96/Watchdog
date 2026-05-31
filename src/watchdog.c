
#define _POSIX_C_SOURCE 200809L

#include <assert.h>	   /*assert*/
#include <fcntl.h>	   /*O_CREAT*/
#include <pthread.h>   /*pthred_t*/
#include <semaphore.h> /*sem_t*/
#include <signal.h>	   /*SIGUSR1,SIGUSR2*/
#include <stddef.h>	   /*size_t*/
#include <stdio.h>	   /*sprintf*/
#include <stdlib.h>	   /*malloc*/
#include <sys/wait.h>  /*waitpid*/
#include <unistd.h>	   /*STDOUT_FILENO*/

#include "scheduler.h" /*typedef sched_t and API*/
#include "watchdog_ex.h"	   /*WDWatch, WDRevive api*/
#include "watchdog.h"  /*WDstart, WDStop api*/

/************************************************ Functions Declarations *****/
static int SendSignal(void* parma);
static void* CreateWatchDog(void* arg);
static void ReviveWD(void* param);
static void SignalHandler(int signo, siginfo_t* info, void* context);
static void StopHandler(int signo, siginfo_t* info, void* context);
static void Cleanup(void* param);
static int CheckSignal(void* param);
static int InitScheduler(wd_t* wd_data);
static void CleanWDData(wd_t* data);

static int PostSem(void* param);

#define UNUSE(x) ((void)(x))
#define STR_LEN (6)
#define WD_EXTRA_ARGS (3)
#define FAILURE (1)
#define TRUE (1)
#define FALSE (0)
#define SEM_PERMISSION (0666)
#define SIZE_T_BUFFER (64)
#define WATCHDOG_PATH ("./watchdog_file.out")
#define SEMAPHORE_NAME ("shared_sem")
#define IS_CHILD (0)
#define SHARED_ALL_THREADS (0)

static pid_t child_pid = 0;
static pthread_t th = 0;
static sig_atomic_t tries = 0;
static sig_atomic_t to_stop = FALSE;

status_t WDStart(size_t tolerance, size_t tolerance_interval, int argc,
				 char** argv)
{
	int i = 0;
	wd_t* wd_data = NULL;

	assert(0 < tolerance_interval);
	assert(0 < argc);
	assert(NULL != argv);

	wd_data = (wd_t*)malloc(sizeof(*wd_data));
	if (NULL == wd_data)
	{
		return WD_ERROR;
	}

	if (WD_ERROR ==
		sem_init(&wd_data->sem_start_program, SHARED_ALL_THREADS, 0))
	{
		CleanWDData(wd_data);

		return WD_ERROR;
	}

	wd_data->sent_argv =
		(char**)malloc((argc + WD_EXTRA_ARGS + 1) * sizeof(char*));
	if (NULL == wd_data->sent_argv)
	{
		CleanWDData(wd_data);

		return WD_ERROR;
	}

	if (FAILURE == InitSignalsActions())
	{
		CleanWDData(wd_data);

		return WD_ERROR;
	}

	for (; i < argc; ++i)
	{
		wd_data->sent_argv[i] = argv[i];
	}

	wd_data->tolerance = tolerance;
	wd_data->interval = tolerance_interval;
	wd_data->argc = argc;
	wd_data->revive_func = ReviveWD;
	if (0 != pthread_create(&th, NULL, CreateWatchDog, wd_data))
	{
		CleanWDData(wd_data);

		return WD_ERROR;
	}

	sem_wait(&wd_data->sem_start_program);

	return WD_SUCCESS;
}

void WDStop(void)
{
	raise(SIGUSR2);
	kill(child_pid, SIGUSR2);

	waitpid(child_pid, NULL, 0);
	pthread_join(th, NULL);
}

void WDRevive(void* param)
{
	wd_t* data = (wd_t*)param;

	assert(NULL != param);
	assert(NULL != data->revive_func);

	data->revive_func(param);
}

void WDWatch(void* wd_data)
{
	wd_t* data = (wd_t*)wd_data;

	assert(NULL != wd_data);

	data->sched = SchedCreate();
	if (NULL == data->sched)
	{
		return;
	}

	if (ERROR == InitScheduler(data))
	{
		SchedDestroy(data->sched);
		return;
	}

	while (0 == to_stop)
	{
		SchedRun(data->sched);
	}

	SchedDestroy(data->sched);
}

int InitSignalsActions()
{
	struct sigaction signal_action = {0};
	struct sigaction stop_action = {0};

	signal_action.sa_sigaction = SignalHandler;
	signal_action.sa_flags = SA_SIGINFO;
	stop_action.sa_sigaction = StopHandler;
	stop_action.sa_flags = SA_SIGINFO;

	if (ERROR == sigaction(SIGUSR1, &signal_action, NULL))
	{
		perror("sigaction child");
		return FAILURE;
	}
	if (ERROR == sigaction(SIGUSR2, &stop_action, NULL))
	{
		perror("sigaction child");
		return FAILURE;
	}

	return SUCCESS;
}
static int SendSignal(void* param)
{
	wd_t* data = (wd_t*)param;

	assert(NULL != param);

	if (data->is_wd)
	{
		write(STDOUT_FILENO, "ping\n", STR_LEN);

		kill(getppid(), SIGUSR1);
	}
	else
	{
		write(STDOUT_FILENO, "pong\n", STR_LEN);

		kill(child_pid, SIGUSR1);
	}

	__sync_fetch_and_add(&tries, 1);

	return SUCCESS;
}

static void* CreateWatchDog(void* arg)
{
	size_t i = 0;
	char tol_str[SIZE_T_BUFFER] = {0};
	char interval_str[SIZE_T_BUFFER] = {0};
	wd_t* wd_data = NULL;

	assert(NULL != arg);

	wd_data = (wd_t*)arg;
	i = wd_data->argc;

	if (wd_data->sem_wd_handshake =
			sem_open(SEMAPHORE_NAME, O_CREAT, SEM_PERMISSION, 0))
	{
	}

	sprintf(tol_str, "%lu", wd_data->tolerance);
	sprintf(interval_str, "%lu", wd_data->interval);

	wd_data->sent_argv[i] = WATCHDOG_PATH;
	wd_data->sent_argv[i + 1] = tol_str;
	wd_data->sent_argv[i + 2] = interval_str;
	wd_data->sent_argv[i + 3] = NULL;

	wd_data->is_wd = FALSE;

	child_pid = fork();
	if (WD_ERROR == child_pid)
	{
		CleanWDData(wd_data);
		return NULL;
	}

	if (0 == child_pid)
	{
		execv(WATCHDOG_PATH, wd_data->sent_argv);
		exit(1);
	}

	sem_wait(wd_data->sem_wd_handshake);
	sem_post(&wd_data->sem_start_program);

	WDWatch(wd_data);

	CleanWDData(wd_data);

	return NULL;
}

static void StopHandler(int signo, siginfo_t* info, void* context)
{
	UNUSE(signo);
	UNUSE(info);
	UNUSE(context);

	__sync_lock_test_and_set(&to_stop, TRUE);
}

static void ReviveWD(void* param)
{
	pid_t new_pid;
	wd_t* data = (wd_t*)param;

	waitpid(child_pid, NULL, WNOHANG);

	assert(NULL != param);

	do
	{
		new_pid = fork();

	} while (WD_ERROR == new_pid);

	if (IS_CHILD == new_pid)
	{
		execv(WATCHDOG_PATH, (char**)data->sent_argv);
		exit(FAILURE);
	}

	child_pid = new_pid;

	sem_wait(data->sem_wd_handshake);

	__sync_lock_test_and_set(&tries, 0);
}

static void SignalHandler(int signo, siginfo_t* info, void* context)
{
	UNUSE(signo);
	UNUSE(info);
	UNUSE(context);

	__sync_lock_test_and_set(&tries, 0);
}

static void Cleanup(void* param) { (void)param; }

static int CheckSignal(void* param)
{
	wd_t* data = (wd_t*)param;
	sched_t* sched = NULL;
	sig_atomic_t tolerance = 0;

	assert(NULL != param);
	assert(NULL != data->sched);

	sched = data->sched;
	tolerance = (sig_atomic_t)data->tolerance;

	if (TRUE == to_stop)
	{
		SchedStop(sched);
	}
	else if (tolerance < tries)
	{
		if (data->is_wd)
		{
			write(STDOUT_FILENO, "reviveWD\n", 10);
		}
		else
		{
			write(STDOUT_FILENO, "reviveU\n", 9);
		}

		__sync_lock_test_and_set(&tries, 0);
		WDRevive(param);
	}

	return SUCCESS;
}

static int InitScheduler(wd_t* wd_data)
{
	assert(NULL != wd_data);

	if (UIDIsSame(
			SchedAdd(wd_data->sched, PostSem, wd_data, Cleanup, NULL, FALSE, 0),
			UIDBadUID))
	{
		return ERROR;
	}

	if (UIDIsSame(SchedAdd(wd_data->sched, SendSignal, wd_data, Cleanup, NULL,
						   TRUE, wd_data->interval),
				  UIDBadUID))
	{
		return ERROR;
	}

	if (UIDIsSame(SchedAdd(wd_data->sched, CheckSignal, wd_data, Cleanup, NULL,
						   TRUE, wd_data->interval),
				  UIDBadUID))
	{
		return ERROR;
	}

	return SUCCESS;
}

static int PostSem(void* param)
{
	wd_t* data = (wd_t*)param;

	assert(NULL != param);

	if (TRUE == data->is_wd)
	{
		write(STDOUT_FILENO, "post\n", STR_LEN);
		sem_post(data->sem_wd_handshake);
	}
	else
	{
		sem_post(&data->sem_start_program);
	}

	return SUCCESS;
}

static void CleanWDData(wd_t* data)
{
	assert(NULL != data);

	sem_destroy(&data->sem_start_program);
	sem_unlink(SEMAPHORE_NAME);
	sem_close(data->sem_wd_handshake);

	free(data->sent_argv);

	free(data);
}