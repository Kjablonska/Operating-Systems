#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <signal.h>
#include <wait.h>

#define PHILOSOPHERS_NB 5
#define RIGHT_FORK (left_fork_id + 1) % PHILOSOPHERS_NB
#define MEALS_NB 2

int philosopher_id;
int sem_id;
int philosopher_pids[PHILOSOPHERS_NB];

void grab_forks(int left_fork_id);
void put_away_forks(int left_fork_id);
void eat();
void think();
void philosopher();
void terminate_philosophers(int failure_number);
void init_philosophers();

/*
	The dining philosophers problem: The philosopher is thinking till the left and right forks are not available. When they are the philosopher takes it and starts eating. Afther that, the philosopher put forks down and starts over.
	Semaphores are integer variables that are used to solve the critical section problem by using two atomic operations, wait and signal that are used for process synchronization.

	Run & Build:
		gcc philosophers.c
		./a.out
*/


/*
	int semop(int semid, struct sembuf *sops, size_t nsops); - performs atomically a user-defined array of semaphore operations on the set of semaphores associated with the semaphore identifier specified by the argument semid.
	The argument sops is a pointer to a user-defined array of semaphore operation structures.

	sops:
		sem_num - semaphore number
		sem_op - semaphore operation:
		sem_flg - operation flag

	If sem_op is negative and sem_flag is 0, semop() will increment the semncnt associated with the specified semaphore and suspend execution of the calling thread untill the value of semval becomes greater than or equal to the absolute value of sem_op.

	If sem_op is negative and sem_flag is 0, semop() will increment the semncnt associated with the specified semaphore.

*/


void grab_forks(int left_fork_id) {
	int right_fork_id = RIGHT_FORK;
	struct sembuf sops[] = {
	        {right_fork_id, -1, 0},
	        {left_fork_id, -1, 0}
	};

	if (semop(sem_id, sops, 2) == -1)
		printf("Philosopher [%d] Error while grabbing forks. \n", philosopher_id);


	printf("Philosopher [%d] takes left[%d] & right[%d].\n", philosopher_id, left_fork_id, right_fork_id);
}


void put_away_forks(int left_fork_id) {
	int right_fork_id = RIGHT_FORK;
	struct sembuf sops[] =
	{
	        {right_fork_id, 1, 0},
	        {left_fork_id, 1, 0}
	};

	if (semop(sem_id, sops, 2) == -1)
		printf("Error %d\n",philosopher_id);


	printf("Philosopher [%d] puts away: left [%d] & right [%d].\n", philosopher_id,left_fork_id,right_fork_id);
}

void eat() {
	printf("Philosopher [%d] is eating.\n", philosopher_id);
	sleep(1);
}

void think() {
	printf("Philosopher [%d] is thinking.\n", philosopher_id);
	sleep(2);
}

void philosopher() {
	for (int i = 0; i < MEALS_NB; i++) {
		think();
		sleep(1);
		grab_forks(philosopher_id);
		sleep(0);
		eat();
		put_away_forks(philosopher_id);
	}
}

void terminate_philosophers(int failure_number) {
	for(int i = 0; i < failure_number; i++)
        kill(philosopher_pids[i], SIGTERM);

}

/*
	int semget(key_t key, int num, int flags); - Creates a new set of semaphores or to locate an existing set based on a key. Returns the System V semaphore set identifier associated with the argument key.
	Returns -1 on failure.

	0644 - permissions read & write -rw-rw-rw-
	IPC_PRIVATE - key_t type, creates new shared memory segment.

	int semctl(int semid, int semnum, int cmd, ...); - Performs semaphore control operations.
*/

void init_philosophers() {
	if ((sem_id = semget(IPC_PRIVATE, PHILOSOPHERS_NB, 0666)) == -1) {
		printf("Cannot create semaphore.\n");
		exit(1);
	}

	// Set the semaphore value (semval) to 1.
	for (int i = 0; i < PHILOSOPHERS_NB; i++) {
		if (semctl(sem_id, i, SETVAL, 1) == -1) {
			printf("Cannot set semaphore.\n");
			exit(1);
		}
	}
}


int main() {
	pid_t pid;
	int status;

	init_philosophers();

	for (int i = 0; i < PHILOSOPHERS_NB; i++) {
		pid = fork();
		if (pid == 0) {
			philosopher_pids[i] = getpid();
			philosopher_id = i;
			philosopher();
			exit(0);
		}
		else if (pid < 0) {
			printf("Something went wrong");
			terminate_philosophers(i);
			exit(1);
		}
	}

	// Waits for children processes.
	for (;;) {
		pid = wait(&status);
		if (pid == -1)
			break;

	}

	// Call semctl with cmd = IPC_RMID.
	// This immediately remove the semaphore set, awakening all processes blocked in semop calls on the set.
	if (semctl(sem_id, 0, IPC_RMID) == -1){
        printf("Error while removing the semaphore.\n");
        exit(1);
    }

	exit(0);
}

