/*
 * signal_block.c
 *
 *  Created on: May 13, 2019
 *      Author: sualeh
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signo) {
	char buff[1024];
	if (signo == SIGINT) {
		int no = sprintf(buff, "Received signal interrupt!\n");
		no = write(STDOUT_FILENO, buff, no);
	} else if (signo == SIGTSTP) {
		int no = sprintf(buff, "Received signal stop!\n");
		no = write(STDOUT_FILENO, buff, no);

	}
}

int main(int argc, char **argv) {
	char buff[1024];
	int no;
	if (signal(SIGINT, signal_handler) == SIG_ERR ) {
		perror("signal");
	s	exit(1);
	}
	if (signal(SIGTSTP, signal_handler) == SIG_ERR ) {
		perror("signal");
		exit(1);
	}

	sigset_t myset, oldset;
	sigemptyset(&myset);
	sigemptyset(&oldset);

	sigaddset(&myset, SIGINT);
	sigaddset(&myset, SIGTSTP);

	sigprocmask(SIG_BLOCK, &myset, &oldset);

	sleep(10); // dont bother me with SIGINT

	sigprocmask(SIG_SETMASK, &oldset, NULL);

	// retrieve signals
	sigemptyset(&myset);
	no = sigpending(&myset);
	if (no == -1) {
		perror("sigpending:");
		exit(1);
	}

	if (sigismember(&myset, SIGTSTP)) {
		int no = sprintf(buff, "SIGSTOP WHILE BLOCKED\n");
		no = write(STDOUT_FILENO, buff, no);
	}
	if (sigismember(&myset, SIGINT)) {
		int no = sprintf(buff, "SIGINT WHILE BLOCKED\n");
		no = write(STDOUT_FILENO, buff, no);
	}
	while (1) {
		// busy waiting, you can bother me now
	}
}
