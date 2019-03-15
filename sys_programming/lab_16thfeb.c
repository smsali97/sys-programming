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
#include <sys/types.h>e
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv) {
	int fd = open("mytext.txt", O_WRONLY);

	if (fd == -1) {
		perror("OPEN ERROR");
	}

	int ret;

	ret = write(fd, "Hi\n", 3);

	if (ret == -1) {
		perror("WRITE ERROR");
	}

	ret = execl("sad", "7", (char*)  NULL);

	if (ret == -1) {
		perror("EXEC ERROR");
	}
	return 0;
}


