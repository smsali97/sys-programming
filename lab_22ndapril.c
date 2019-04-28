/*
 * lab_22ndapril.c
 *
 *  Created on: Apr 22, 2019
 *      Author: sualeh
 *
 *
 * Using select
 */

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define TIMEOUT 5
#define BUF_LEN 1024

int main(int argc, char **argv) {
	struct timeval tv;
	fd_set readfds;
	int ret;

	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO,&readfds);

	// waiting
	ret = select(STDIN_FILENO + 1, &readfds,NULL,NULL,&tv);

	if (ret == -1) {
		perror("select: ");
		exit(1);
	} else if (!ret) {
		return TIMEOUT;
	}

	if (FD_ISSET(STDIN_FILENO, &readfds)) {
		char buf[BUF_LEN+1];
		int len;
	len = read(STDIN_FILENO,buf,BUF_LEN);
	if (len == - 1) {
		perror("read: ");
		return 1;
	}

	if (len) {
		buf[len] = '\0';
		write(STDOUT_FILENO,buf,len);
	}

	}
}
