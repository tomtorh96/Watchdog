                +--------------------------+
                |       User Program       |
                |       (WDStart call)     |
                +------------+-------------+
                             |
                             | creates thread
                             v
                +--------------------------+
                |   Watchdog Thread (WD)   |
                |   (inside user process)  |
                +------------+-------------+
                             |
                             | fork + exec
                             v
                +---------------------------+
                |   Watchdog Process        |
                |   (watchdog_file.out)     |
                +---------------------------+

------------------------------------------------------------

        HEARTBEAT COMMUNICATION (SIGUSR1)

        User Process  <------------------>  Watchdog Process
             |                                   	|
             |                                		|
       sends SIGUSR1                 	       sends SIGUSR1
       every interval                   	   every interval
             |                                 		|
             v                               		v
        tries++                      			  tries++

        On receive SIGUSR1:
        tries = 0                       		 tries = 0

------------------------------------------------------------

        FAILURE DETECTION

        If tries >= Tolerance:

        User detects WD dead:
            → fork + exec → restart watchdog process

        Watchdog detects User dead:
            → exec → restart user process

------------------------------------------------------------

        STOP FLOW (WDStop)

        User calls WDStop()
                |
                v
        send SIGUSR2 to watchdog
                |
                v
        Watchdog receives SIGUSR2
                |
                v
        Cleanup:
            - stop scheduler
            - close semaphores
            - exit

------------------------------------------------------------

        SYNCHRONIZATION

        Named Semaphores:
            - shared_sem

	Unnamed Semaphores:
	   -sem_start_program

        Used for:
            - startup synchronization
            - ensuring both sides are ready

------------------------------------------------------------

        SUMMARY

        User Process:
            - Main program
            - Watchdog thread
            - Scheduler
            - Signal handlers

        Watchdog Process:
            - Separate executable
            - Scheduler
            - Signal handlers

        Shared Mechanisms:
            - Signals (SIGUSR1, SIGUSR2)
            - Named semaphore
