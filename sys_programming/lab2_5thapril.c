/*
 * lab2_5thapril.c
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

int main(int argc, char **argv) {
	int x = fork();
	int y = fork();
	int status;
	int ret;
	char buff[256];
	if ( x > 0 || y > 0) {

		ret = waitpid(0,&status, WNOHANG);
		if ( ret == -1) {
			perror("wait");
		}
		ret = sprintf(buff,"%d\n",ret);
		write(STDOUT_FILENO,buff,ret);
//		exit(0);
	}
}

