/*
 * lab_16thfeb.c
 *
 *  Created on: Feb 16, 2019
 *      Author: sualeh
 *
 *  Using exec to pass argument to another process (lab2_16thfeb.c)
 */

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int fd = open("mytext.txt", O_WRONLY);

	if (fd == -1) {
		perror("OPEN ERROR");
		exit(1);
	}

	int ret;

	ret = write(fd, "Hi\n", 3);

	if (ret == -1) {
		perror("WRITE ERROR");
		exit(2);
	}

	ret = execl("sad", "7", (char*)  NULL);

	if (ret == -1) {
		perror("EXEC ERROR");
		exit(3);
	}
	return 0;
}


