#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define N	        5
#define LEFT	    (i + N - 1 ) % N
#define RIGHT	    ( i + 1 ) % N

#define THINKING    0
#define HUNGRY      1
#define EATING      2

pthread_mutex_t	m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t	s[N] = PTHREAD_MUTEX_INITIALIZER;
pthread_t philosophers_threads_id[N];
int state[N];

void grab_forks(int philo_id);
void put_away_forks(int philo_id);
void test(int philo_id);

int main() {

}

void grab_forks( int philo_id ) {

}

void put_away_forks( int philo_id ) {

}

void test(int philo_id) {

}