#ifndef __WATCHDOG_EX_H__
#define __WATCHDOG_EX_H__

typedef struct WDData
{
	sem_t sem_start_program;
	sched_t* sched;
	sem_t* sem_wd_handshake;
	char** sent_argv;
	size_t tolerance;
	size_t interval;
	size_t argc;
	int is_wd;
	void (*revive_func)(void*);
} wd_t;

void WDRevive(void* param);

void WDWatch(void* wd_data);

int InitSignalsActions(void);

#endif /*__WATCHDOG_EX_H__*/
