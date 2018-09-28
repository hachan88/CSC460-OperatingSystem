/*
Excercise 4.20
Modify programming problem Exercise 3.20 from Chapter 3, which asks you to design a pid manager. 
This modification will consist of writing a multithreaded program that tests your solution to Exercise 3.20. 
You will create a number of threads — for example, 100 — and each thread will request a pid, 
sleep for a random period of time, and then release the pid. (Sleeping for a random period of time approximates the typical pid usage in which a pid is assigned to a new process, the process executes and then terminates, and the pid is released on the process’s termination.) 
On UNIX and Linux systems, sleeping is accomplished through the sleep() function, which is passed an integer value representing the number of seconds to sleep. 
*/

/*
Excercise 3.20
An operating system’s pid manager is responsible for managing process identifiers. 
When a process is first created, it is assigned a unique pid by the pid manager. 
The pid is returned to the pid manager when the process completes execution, and the manager may later reassign this pid. 
Process identifiers are discussed more fully in Section 3.3.1. What is most important here is to recognize that process identifiers must be unique; no two active processes can have the same pid.
Use the following constants to identify the range of possible pid values:
#define MIN PID 300 #define MAX PID 5000
You may use any data structure of your choice to represent the avail- ability of process identifiers. 
One strategy is to adopt what Linux has done and use a bitmap in which a value of 0 at position i indicates that a process id of value i is available and a value of 1 indicates that the process id is currently in use.
Implement the following API for obtaining and releasing a pid:
• int allocate map(void)—Createsandinitializesadatastructure for representing pids; returns—1 if unsuccessful, 1 if successful
• int allocate pid(void) — Allocates and returns a pid; returns — 1 if unable to allocate a pid (all pids are in use)
• void release pid(int pid)—Releases a pid
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define MIN_PID 300
#define MAX_PID 5000

/*data structure to represent the availability of process identifiers*/
struct pids {
	int pid;
	int available;
} *p;

/*Creates and initializes a data structure for representing pids*/
/*returns—1 if unsuccessful, 1 if successful*/
int allocate_map(void)
{

	p = (struct pids *)calloc((MAX_PID - MIN_PID + 1), sizeof(struct pids));

	if (p == NULL)
		return -1;

	int i, j;
	for (i = MIN_PID, j = 0; i <= MAX_PID; i++, j++) {
		p[j].pid = i;
		p[j].available = 0;
	}
	return 1;
}

/*Allocates and returns a pid*/
/*return -1 if unable to allcoate pid*/
int allocate_pid(void)
{
	int i, j;
	for (i = MIN_PID, j = 0; i <= MAX_PID; i++, j++) {
		if (p[j].available == 0) {
			p[j].available = 1;
			printf("Allocated pid: %d\n", p[j].pid);
			return p[j].pid;
		}
	}
	return -1;
}

/*Releases a pid*/
void release_pid(int pid)
{
	p[pid - MIN_PID].available = 0;
	printf("Released pid: %d\n", pid);
}

void *thread_func(void *param)
{
	int process;

	process = allocate_pid();
	sleep(rand()%10+2);
	release_pid(process);
	pthread_exit(0);
}

int main(int argc, char *argv[])
{
	int i, numthreads;
	srand(time(NULL));

	if (argc > 1)
		numthreads = atoi(argv[1]);
	else
		numthreads = 2;

	pthread_t *threadptr;
	threadptr = malloc(sizeof(pthread_t)*numthreads);

	allocate_map();

	for (i = 0; i < numthreads; i++)
		pthread_create(threadptr + i, NULL, &thread_func, NULL);

	for (i = 0; i < numthreads; i++)
		pthread_join(*(threadptr + i), NULL);

	free(threadptr);
	return 0;
}
