#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#define PHILOSOPHERS_NB 5
#define LEFT_FORK_ID 0
#define RIGHT_FORK_ID 0

pthread_t phiolsophers_threads_ids[PHILOSOPHERS_NB];

void grab_forks(int left_fork_id);
void put_away_forks(int left_fork_id);
void eat(int philosopher_id);
void think(int philosopher_id)



void main() {

}


void grab_forks(int left_fork_id) {
    int philosopher_id = left_fork_id;

    
}

void put_away_forks(int left_fork_id) {

}

void eat(int philsopher_id) {
    sleep(2);
    printf("Philosopher %d is eating with forks: left [%d]  right [%d]", philsopher_id, LEFT_FORK_ID, RIGHT_FORK_ID);
}

void think(int phiolosopher_id) {
    printf("Philosopher %d is thinking", phiolosopher_id);
}

void *philosopher(void *id) {
    int philosopher_id = *(int *) id;
    while (1) {
        think(philosopher_id)
        grab_forks(philosopher_id)
        eat(philosopher_id)
        put_away_forks(philosopher_id)
    }
}