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
