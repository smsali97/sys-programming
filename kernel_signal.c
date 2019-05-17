/*
 * kernel_signal.c
 *
 *  Created on: May 16, 2019
 *      Author: sualeh
 *
 *
 *      FPE signal isnt kernel signal apparently
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


void advanced_handler(int signo, siginfo_t *si, void *ucontext) {

	if (si->si_code == FPE_INTDIV) {
		printf("div by zero\n");
	}

	if (si->si_code == SI_KERNEL) {
		printf("I'm from kernel\n");
	}
	printf("My pid is %d and my time was %f seconds",si->si_pid,si->si_utime);

	exit(1);
}

int main(int argc, char **argv) {
	int no1 = atoi(*(++argv));
	int no2 = atoi(*(++argv));



	// override signal
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = &advanced_handler;
	sa.sa_flags = SA_SIGINFO;

	sigaction(SIGFPE,&sa,NULL);

	printf("The division is %d",no1/no2);
}
