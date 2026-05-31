    watchdog
------------------

Overview: The system utilizes a mutual-monitoring "ping-pong" architecture where
          a User Process and a Watchdog Process exchange SIGUSR1 signals to verify
          each other's status and uses SIGUSER2 to signals to stop checking his partner.
          If one process fails, the survivor uses a fork/exec mechanism to revive his partner.

Goals: The primary objective is to provide high availability and fault tolerance,
       ensuring mission-critical applications can recover from crashes automatically.
       It is also designed to preserve the original execution arguments of the application during revival.

Usage: Developers integrate the system by calling WDStart with specified tolerance and interval parameters.
       The Watchdog then manages monitoring in a background thread while the main application logic continues.
       Proper cleanup and termination are handled via the WDStop function.

compiling and running: The user file and watchdog_file needs to be in the same folder.
                       The MakeFile needs also to be or in the same folder
                       as the user file, watchdog file, watchdog.c and smokey.h, or be in a folder above them.
                       on the user run command "make <user file name>" no need for file type.
                       In the location of the makefile there will be an object folder (obj), library folder (lib)
                       and an executable file of both the user file and watchdog_file.
                       The user only need now to run is executable file of his file.
