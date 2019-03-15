/*
 * pipe_practice.c
 *
 *  Created on: Mar 6, 2019
 *      Author: sualeh
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>


int my_add(int x);

const int BUF_SIZE = 256;

int main(int argc, char **argv) {

	int fd1[2];
	int fd2[2];


	int noOfBytes;
	char buf[BUF_SIZE];

	if (pipe(fd1) < 0) {
		perror("PIPE 1 FAILED");
		exit(1);
	}
	if (pipe(fd2) < 0) {
		perror("PIPE 2 FAILED");
		exit(2);
	}

	pid_t status = fork();
	if (status < 0) {
		perror("FORK FAILED");
		exit(3);
	}
	// Parent
	else if (status > 0) {
		// Close  Reading End of First Pipe
		close(fd1[0]);
		noOfBytes = read(STDIN_FILENO, buf, BUF_SIZE);
		if (noOfBytes < 0) {
			perror("READ ERROR");
			exit(4);
		}
		noOfBytes = write(fd1[1], buf, noOfBytes);
		if (noOfBytes < 0) {
			perror("WRITE ERROR");
			exit(5);
		}


		// Wait for child
//		noOfBytes = waitpid(status, &status, 0);
//
//		if (noOfBytes < 0){
//			perror("WAIT ERROR");
//			exit(6);
//		}
		close(fd1[1]);


		close(fd2[1]);
		noOfBytes = read(fd2[0], buf, BUF_SIZE);

		if (noOfBytes < 0) {
			perror("READ ERROR");
			exit(7);
		}
		noOfBytes = write(STDOUT_FILENO, buf, noOfBytes);
		if (noOfBytes < 0) {
			perror("WRITE ERROR");
			exit(8);
		}
	}
	// Child
	else {
		// Close Writing End of First Pipe
		close(fd1[1]);
		noOfBytes = read(fd1[0],buf,BUF_SIZE);
		if (noOfBytes < 0) {
			perror("READ ERROR");
			exit(7);
		}

		char* str;
		sscanf(buf, "%[^\n]", str);
		int x = atoi(strtok(str," "));
		printf("%d",x);
		x = my_add(x);

		close(fd1[0]);


		close(fd2[0]);
		int noOfBytes = sprintf(buf, "Number of arguments read: %d\n",x);
		noOfBytes = write(fd2[1],buf,noOfBytes);
		if (noOfBytes < 0 ) {
			perror("WRITE");
			exit(8);
		}
		close(fd2[1]);

	}

}


int my_add(int x) {
	char buff[BUF_SIZE];
	int no;
	char* token;

	// Float to store result with temporary number;
	double result = x;
	double value;
	int ctr = 1;
	// Get Next Tokens
	while ((token = strtok(NULL, " ")) != NULL) {

		// Input Checking
		for (int i = 0; token[i] != '\0'; i++) {
			if (!isdigit(token[i])) {
				no = sprintf(buff,
						"There was a non-digit character '%c' in your input\n",
						token[i]);
				write(STDOUT_FILENO, buff, no);
			}
		}

		// perform relevant operation
		value = atoi(token);
		result += value;
		ctr++;
	}

	no = sprintf(buff, "The answer is: %.2f \n", result);
	write(STDOUT_FILENO, buff, no);

	return ctr;

}

