/*
 * lab2_6thapril.c
 *
 *  Created on: Apr 6, 2019
 *      Author: sualeh
 */

/*
 * lab_5thapril.c
 *
 *  Created on: Apr 5, 2019
 *      Author: sualeh
 */

#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>


void signal_handler(int sig_no);


int main(int argc, char **argv) {
	int x = 1;
	int status = -1;
	int no = signal(SIGINT,signal_handler);
	char buff[2450];

	if ( no == -1) {
		perror("signal");
		exit(0);
	}
	x = fork();
	if ( x== -1) {
		perror("fork");
		exit(0);
	}
	if ( x > 0) {
		int l = waitpid(x,&status,0);
		if (l == -1) {
			perror("wait");
		}
		printf("%d!!",status);
	}
	else {

		kill(getppid(),SIGINT);
		read(STDIN_FILENO,buff,1000);
	}

}

void signal_handler(int sig_no) {
	char buff[256];
	int x = sprintf(buff,"%d",getpid());
	write(STDOUT_FILENO,buff,x);
}



