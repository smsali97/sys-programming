/*
 * lab_23feb.c
 *
 *  Created on: Feb 23, 2019
 *      Author: sualeh
 */
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

int main(void) {

	const int SIZE = 1000;
	char buff[SIZE];
	ssize_t ret;

	int no;

	char* inFileName = "in.txt";

	// The file to be read
	int sfd = open(inFileName, O_WRONLY);
	if (sfd == -1) {
		perror("Error");
		return 3;
	}

	while (true) {


		int forkStatus = fork();
		if (forkStatus == -1) {
			perror("FORK");
			exit(1);
		}
		if (forkStatus == 0) {
			no = read(STDIN_FILENO, buff, SIZE);
			if (no == -1) {
				perror("READ: ");
				exit(5);
			}

			no = write(sfd, buff, no);
			if (no == -1) {
				perror("WRITE");
				exit(6);
			}
		}

		else {

			int dfd = open(inFileName, O_WRONLY);
			if (dfd == -1) {
				perror("OPEN");
			}

			ret = read(dfd, buff, SIZE);

			char str[ret - 1];
			// Delimiter for token breaking
			const char delimiter[1] = " ";
			char* token;
			sscanf(buff, "%[^\n]", str);

			// Get first token
			token = strtok(str, delimiter);

			// Evaluate token
			double result = atoi(token);

			token = strtok(str, NULL);
			printf("%s", token);

			result += atoi(token);

			printf("%d", result);
		}
	}
}

