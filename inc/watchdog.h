#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

typedef enum
{
	WD_ERROR = -1,
	WD_SUCCESS,
	WD_FAILURE
} status_t;

/**
 * Description: Starts a watchdog mechanism that ensures the calling process
 * 				("client") remains alive. A dedicated watchdog process is
 * 				created and synchronized with a watchdog thread running inside
 * 				the client process.
 *
 * Arguments:	interval  - Time interval (in seconds or scheduler units)
 * 							between heartbeats. Must be > 0.
 *
 * 				tolerance - Maximum number of missed heartbeats allowed
 * 							before revival. Must be > 0.
 *
 * 				argc      - Argument count of the client program.
 *							Must match argv.
 *
 * 				argv      - Argument vector of the client program.
 *							Used for re-executing the client process.
 *							Must not be NULL.
 *
 * Return:		WD_SUCCESS - Watchdog successfully started and synchronized.
 * 				WD_ERROR   - Failure in initialization.
 *
 * Complexity:  Time:  O(1) (initialization only; runtime behavior is ongoing).
 *              Space: O(1)
 *
 * Requirements / Assumptions:
 * - The program must be linked with pthread and semaphore support.
 * - The watchdog executable must exist at the defined path (WATCHDOG_APP).
 * - Signals SIGUSR1 and SIGUSR2 must not be overridden by user code.
 * - Environment allows fork() and exec().
 * - Calling once per process lifecycle.
 * - Running in environments that support POSIX APIs.
 *
 * Forbidden Usage:
 * - Calling WDStart multiple times without calling WDStop.
 * - Using interval == 0 or tolerance == 0.
 * - Modifying environment variables used internally:("WD_PID", semaphores
 * names).
 * - Using the same semaphore names from unrelated programs. (user_sem,
 * thread_sem).
 *
 * Failure Cases:
 * - pthread_create fails
 * - sem_open fails
 * - fork fails
 * - execvp fails
 * - scheduler creation fails
 *
 * In such cases:
 * - Resources may be partially allocated
 * - Function returns WD_ERROR
 *
 * Notes:
 * - This function blocks until initial synchronization is complete.
 * - After return, watchdog is fully operational.
 */
status_t WDStart(size_t interval, size_t tolerance, int argc, char** argv);

/**
 * Description: Stops the watchdog system and shuts everything down cleanly.
 *
 * Arguments: - None.
 *
 * Return:    - None.
 *
 * Complexity:  Time:  O(1)
 *              Space: O(1)
 */
void WDStop(void);

#endif /*__WATCHDOG_H__*/