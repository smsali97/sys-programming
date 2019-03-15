/*
 * lab3rdfeb.c
 *
 *  Created on: Mar 2, 2019
 *      Author: sualeh
 *
 *    Using waitpid to return statuses to child
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char argv[]) {

	int status;
	pid_t pid;
	pid = fork();
	if (pid == -1)
		return -1;
	else if (pid == 0) {
		execv("/usr/bin/gedit", NULL);
	}
	if (waitpid(pid, &status, 0) == -1)
		return -1;
	printf("%d\n",status);
	if (WIFEXITED(status))
		printf("%d\n",WEXITSTATUS(status));

}

