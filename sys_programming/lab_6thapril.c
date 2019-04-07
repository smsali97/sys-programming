/*
 * lab_6thapril.c
 *
 *  Created on: Apr 6, 2019
 *      Author: sualeh
 *
 *  What is the status of the program when it closes by the red x
 *  Answer: 0
 *
 *  Kill?
 *  Nothing
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

int main(int argc, char **argv) {
	int pid = fork();
	int status;
	char buff[240];
	if ( pid > 0) {
		int wpid = waitpid(pid,&status,0);
		if (wpid == -1) {
			perror("wait");
			exit(1);
		}
		if (WIFEXITED(status)) {
			int no = sprintf(buff,"%d\n",WEXITSTATUS(status));
			no = write(STDOUT_FILENO,buff,no);

		}
	}
	else {
		int no = sprintf(buff,"%d\n",getpid());
		no = write(STDOUT_FILENO,buff,no);
		status = execlp("gedit",NULL);
		if (status < 0) {
			perror("exec");
			exit(1);
		}
	}
}
