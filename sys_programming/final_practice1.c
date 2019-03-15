/*
 * final_practice1.c
 *
 *  Created on: Mar 15, 2019
 *      Author: sualeh
 */
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
	const int BUFF_SIZE = 100;
	char buff[BUFF_SIZE];

	int no;
	int fd;
	char* str;
	fd = open("y", O_RDONLY);
	if (no == -1) {
		printf("%d\n", errno);
		perror("error:");
		exit(1);
	}
	no = read(fd, buff, BUFF_SIZE);
	if (fd == -1) {
		printf("%d\n", errno);
		perror("error:");
		exit(1);
	}
	no = write(STDOUT_FILENO, buff, no);
	if (no == -1) {
		printf("%d\n", errno);
		perror("error:");
		exit(1);
	}
	no = lseek(fd, 0, SEEK_SET);
	if (no == -1) {
		printf("%d\n", errno);
		perror("error:");
		exit(1);
	}
	no = read(fd, buff, BUFF_SIZE);
	if (fd == -1) {
		printf("%d\n", errno);
		perror("error:");
		exit(1);
	}
	no = write(STDOUT_FILENO, buff, no);
	if (no == -1) {
		printf("%d\n", errno);
		perror("error:");
		exit(1);
	}
	close(no);
}

