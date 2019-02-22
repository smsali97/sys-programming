/*
 * milestone3.c
 *
 *  Created on: Feb 21, 2019
 *      Author: sualeh
 */

#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int SIZE = 100;
const char add[] = "add";
const char run[] = "run";
const char quit[] = "exit";

// Number of bytes read/written
int no;

// Delimiter for token breaking
const char delimiter[1] = " ";
char* token;

// prototypes of my function
void my_add();
void my_run();

int main(void) {
	char buff[SIZE];

	while (true) {
		// picks number of bytes from buffer and reads it
		no = read(STDIN_FILENO, buff, SIZE);

		// Error handling
		if (no == -1) {
			perror("READ ERROR: ");
			exit(1);
		}

		char str[no - 1];
		sscanf(buff, "%[^\n]", str);

		// Get first token
		token = strtok(str, delimiter);

		// Evaluate token
		if (strcmp(token, add) == 0) {
			my_add();
		} else if (strcmp(token, run) == 0) {
			my_run();
		} else if (strcmp(token, quit) == 0) {
			no = sprintf(buff, "Bye!\n");
			write(STDOUT_FILENO, buff, no);
			return 0;
		} else {
			no = sprintf(buff, "Invalid Command \n");
			write(STDOUT_FILENO, buff, no);
		}

	}
}

void my_add() {
	char buff[SIZE];

	// Float to store result with temporary number;
	double result = 0;
	double value;

	// Get Next Tokens
	while ((token = strtok(NULL, delimiter)) != NULL) {

		// Input Checking
		for (int i = 0; token[i] != '\0'; i++) {
			if (!isdigit(token[i])) {
				no = sprintf(buff,
						"There was a non-digit character '%c' in your input\n",
						token[i]);
				write(STDOUT_FILENO, buff, no);
				return;
			}
		}

		// perform relevant operation
		value = atoi(token);
		result += value;
	}

	no = sprintf(buff, "The answer is: %.2f \n", result);
	write(STDOUT_FILENO, buff, no);

}

void my_run() {
	char buff[SIZE];

	char* next_token = strtok(NULL, delimiter);
	if (next_token == NULL) {
		no = sprintf(buff, "No argument provided! \n");
		write(STDOUT_FILENO, buff, no);
		return;
	}

	no = fork();
	if (no == -1) {
		perror("FORK FAILED");
		return;
	}
	// child process
	else if (no == 0) {
		no = sprintf(buff, "My id is %d\nMy parent's id is %d\n", getpid(),
				getppid());
		write(STDOUT_FILENO, buff, no);
		no = execlp(next_token, NULL);
		if (no == -1) {
			if (errno == ENOENT) {
				no = execl(next_token, NULL);
				{
					perror("EXECL FAILED TOO: ");
					return;
				}
			} else {
				perror("EXECLP FAILED");
				return;
			}
		}

	}
	// parent process
	else {
		no = sprintf(buff, "My id is %d\nMy parent's id is %d\n", getpid(),
				getppid());
		write(STDOUT_FILENO, buff, no);
	}

}
