/*
 * detach_threads.c
 *
 *  Created on: May 15, 2019
 *      Author: sualeh
 *
 *  One thread waiting for the other to give output
 *
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* thread_func() {
	int *i = malloc(sizeof(int));
	for (*i = 0; *i < 100; (*i)++) {

	}
	(*i)++;
	pthread_exit((void*) i);
}

int main(int argc, char **argv) {
	pthread_t t1;
	pthread_attr_t at1;

	// Detached thread: option 1
	pthread_attr_init(&at1);
//	pthread_attr_setdetachstate(&at1,PTHREAD_CREATE_DETACHED);


	pthread_create(&t1,&at1,&thread_func,NULL);
	// Detached thread: option 2
//	pthread_detach(t1);


	int *num;
	int ret = pthread_join(t1,(void *)&num);
	if (ret == -1) {
		perror("pthread_join: ");
		exit(1);
	}

	printf("The number I got was %d\n",*num);
}
