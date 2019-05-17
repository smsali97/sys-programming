/*
 * signal_payload.c
 *
 *  Created on: May 13, 2019
 *      Author: sualeh
 *
 *  Child sends signal to parent with custom payload
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void advanced_handler(int signo, siginfo_t *si, void *ucontext) {
	char buff[1024];

	int no = sprintf(buff,"I love %s\n",(char*)si->si_ptr);
	if (si->si_code == SI_QUEUE) {
		no += sprintf(buff + no,"I got sent by user\n");
	}
	no = write(STDOUT_FILENO,buff,no);
}


int main(int argc, char **argv) {

	// setup
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = &advanced_handler;
	sa.sa_flags = SA_SIGINFO;

	sigaction(SIGUSR1,&sa, NULL);
	int x = fork();

	if (x == 0) {
		// child

		union sigval s;
		s.sival_ptr = (void *) "Chicken Biryani";

		sigqueue(getppid(), SIGUSR1, s);
	} else {
		// parent
		while (1) {

		}

	}
}
