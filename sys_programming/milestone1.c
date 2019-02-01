/*
 * milestone1.c
 *
 *  Created on: Jan 29, 2019
 *      Author: sualeh
 */
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv) {
	const int SIZE = 100;
	char buff[SIZE];

	bool addFlag;

	while (true) {
		// picks number of bytes from buffer and reads it
		int no = read(STDIN_FILENO, buff, SIZE);
		//x = write(STDOUT_FILENO, buff, no);

		// Error handling
		if (no == 0) {
			perror("ERROR: ");
			return -1;
		}

		// To take input
		char str[no-1];
		// Delimiter for token breaking
		const char delimiter[2] = " ";
		char* token;
		sscanf(buff, " %[^\n]", str);

		// Get first token
		token = strtok(str, delimiter);

		// Check what operation to do
		if (strcmp(token, "add") == 0)
			addFlag = 1;

		// Float to store result with initial number
		double result = 0;
		double value;

		// Get Next Tokens
		while ((token = strtok(NULL, delimiter)) != NULL) {
			// perform relevant operation
			value = atoi(token);
			if (addFlag)
				result += value;
		}

		no = sprintf(buff, "The answer is: %.2f \n",result);
		write(STDOUT_FILENO, buff, no);
	}
}
