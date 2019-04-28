/*
 * milestone4.c
 *
 *  Created on: Feb 26, 2019
 *      Author: sualeh
 */

#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFF_SIZE 30

void my_add(char buf[], int index);

int main(int argc, char **argv) {
	int no = 0;
	char buf[BUFF_SIZE];
	char buf2[BUFF_SIZE];
	const char endLine = ';';

	const char delimiter[1] = " ";
	char* token;
	bool endFound = false;
	bool hasRead = false;
	int index;
	int totBytes = 0;
	int addX = 0;

	while (true) {

		if (totBytes == no - 1) {
			totBytes = 0;
			endFound = false;
		}

		// If encountered an endLine before, chances are some input is remaining
		if (endFound) {
			// Shift the buffer left 'index'
			for (int i = index + 1; i < (index + 1) + no; i++)
				buf[i - (index + 1)] = buf[i];
		} else {
			// Read 'no' bytes from stdin
			no = read(STDIN_FILENO, buf + addX, BUFF_SIZE);
			if (no == -1) {
				perror("READ ERROR");
			}
			hasRead = true;
		}

		// Find end line!
		index = -1;
		for (int i = 0; i < no; i++) {
			if (buf[i] == endLine) {
				endFound = true;
				index = i;
				totBytes += index + 1;
				addX = 0;
				break;
			}
		}
		if (endFound) {
			my_add(buf, index);
		} else {
//			index = no - 1;
			addX += no - 1;
		}
	}
}

void my_add(char buf[], int index) {
	int no;

	bool errFlag = false;
	char str[index];
// Delimiter for token breaking
	const char delimiter[1] = " ";
	char* token;
	sscanf(buf, "%[^;]", str);
	char buf2[BUFF_SIZE];

// Get first token
	token = strtok(str, " ");

// Evaluate token
	double result = atoi(token);

// Terminate condition

// Float to store result with temporary number;
	double value;

// Get Next Tokens
	while ((token = strtok(NULL, delimiter)) != NULL) {

		for (int i = 0; token[i] != '\0'; i++) {
			if (!isdigit(token[i])) {
				errFlag = 1;
				break;
			}
		}

		if (errFlag) {
			no = sprintf(buf2, "There's a non-digit token!\n");
			write(STDOUT_FILENO, buf2, no);
		}

		// perform relevant operation
		value = atoi(token);
		result += value;
	}

	if (!errFlag) {

		no = sprintf(buf2, "The answer is: %.2f \n", result);
		write(STDOUT_FILENO, buf2, no);
	}
}

