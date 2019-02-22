/*
 * milestone2.c
 *
 *  Created on: Feb 7, 2019
 *      Author: sualeh
 *  this assignment is related to observing performance difference while writing in files
 *  (file copy as an example) across syncing and non-syncing and different buffer sizes.

 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char **argv) {

	// Take source filename input

	const int SIZE = 1000;
	char buff[SIZE];

	int no = sprintf(buff, "Please enter source file\n");
	write(STDOUT_FILENO, buff, no);

	no = read(STDIN_FILENO, buff, SIZE);
	if (no == -1) {
		perror("Error");
		return 1;
	}
	char inFileName[no - 1];
	sscanf(buff, "%[^\n]", inFileName);

	// Take destination filename input

	no = sprintf(buff, "Please enter destination file\n");
	write(STDOUT_FILENO, buff, no);

	no = read(STDIN_FILENO, buff, SIZE);
	if (no == -1) {
		perror("Error");
		return 2;
	}
	char outFileName[no - 1];
	sscanf(buff, "%[^\n]", outFileName);

	// The file to be read
	int sfd = open(inFileName, O_RDONLY);
	if (sfd == -1) {
		perror("Error");
		return 3;
	}

	// The file to be written
	int dfd = open(outFileName, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if (dfd == -1) {
		perror("Error");
		return 4;
	}

	// Reading buffer
	int BUFF_SIZE = 1000;
	ssize_t ret;

	// START TIME
	clock_t tic = clock();

	while ((ret = read(sfd, buff, BUFF_SIZE)) != 0) {
		if (ret == -1) {
			if (errno == EINTR)
				continue;
			else {
				perror("ERROR");
				return 5;
			}
		}
		no = write(dfd, buff, ret);
//		fsync(dfd);
		if (no == -1) {
			perror("ERROR");
			return 6;
		}
	}
	clock_t toc = clock();

	double elapsedTime = (toc - tic) / CLOCKS_PER_SEC;

	no = sprintf(buff, "The 1000 byte buffer took: %f \n", elapsedTime);
	write(STDOUT_FILENO, buff, no);

	// Clean up work
	close(sfd);
	close(dfd);

	// PART 2

	// The file to be read
	sfd = open(inFileName, O_RDONLY);
	if (sfd == -1) {
		perror("Error");
		return 3;
	}

	// The file to be written
	dfd = open(outFileName, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if (dfd == -1) {
		perror("Error");
		return 4;
	}

	// Reading buffer
	BUFF_SIZE = 1;

	// START TIME
	tic = clock();

	while ((ret = read(sfd, buff, BUFF_SIZE)) != 0) {
		if (ret == -1) {
			if (errno == EINTR)
				continue;
			else {
				perror("ERROR");
				return 5;
			}
		}
		no = write(dfd, buff, ret);
//		fsync(dfd);
		if (no == -1) {
			perror("ERROR");
			return 6;
		}
	}
	toc = clock();

	double elapsedTime2 = (toc - tic) / CLOCKS_PER_SEC;

	no = sprintf(buff, "The 1 byte buffer took: %f \n", elapsedTime2);
	write(STDOUT_FILENO, buff, no);

	no = sprintf(buff, "The time difference in seconds is %f \n",
			elapsedTime2 - elapsedTime);
	write(STDOUT_FILENO, buff, no);
	// Clean up work
	close(sfd);
	close(dfd);

	return 0;
}

