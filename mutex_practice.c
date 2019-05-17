/*
 * mutex_practice.c
 *
 *  Created on: May 12, 2019
 *      Author: sualeh
 *
 *  Mutex and Race Conditions
 */

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_LOOPS 200000
#define BUF_SIZE 1024
long long sum = 0; // global variable

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* counting_thread(void *arg) {
	int offset = *(int*) arg;
	for (int i = 0; i < NUM_LOOPS; i++) {
		// start critical section
		pthread_mutex_lock(&mutex);
		 sum += offset;
		// end
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}


int main(int argc, char **argv) {

	char buf[BUF_SIZE];

	// spawn threads
	pthread_t tid;

	int offset1 = 1;
	pthread_create(&tid,NULL, &counting_thread, &offset1);



	pthread_t tid2;
//	sleep(5);
//	if we use the same offset hmmm
// race condition
//	offset1 = -1;

	int offset2 = -1;
	pthread_create(&tid2,NULL, &counting_thread, &offset2);

	// wait for threads to finish
	pthread_join(tid,NULL);
	pthread_join(tid2,NULL);
	int no = sprintf(buf,"Sum = %lld\n",sum);
	no = write(STDOUT_FILENO,buf,no);
}



