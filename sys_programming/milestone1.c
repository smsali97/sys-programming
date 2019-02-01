/*
 * milestone1.c
 *
 *  Created on: Jan 29, 2019
 *      Author: sualeh
 *
 *      Usage: Enter list of numbers to be added separated by spaces
 *      if the first number is zero/any non-numeric word, the program
 *      will terminate.
 */
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	const int SIZE = 100;
	char buff[SIZE];

	while (true) {
		// picks number of bytes from buffer and reads it
		int no = read(STDIN_FILENO, buff, SIZE);

		// Error handling
		if (no == 0) {
			perror("ERROR: ");
			exit(1);
		}

		// To take input
		char str[no - 1];
		// Delimiter for token breaking
		const char delimiter[2] = " ";
		char* token;
		sscanf(buff, "%[^\n]", str);

		// Get first token
		token = strtok(str, delimiter);

		// Evaluate token
		double result = atoi(token);

		// Terminate condition
		if (!result) {
			no = sprintf(buff, "BYE BYE!\n");
			write(STDERR_FILENO, buff, no);
			exit(0);
		}

		// Float to store result with temporary number;
		double value;

		// Get Next Tokens
		while ((token = strtok(NULL, delimiter)) != NULL) {
			// perform relevant operation
			value = atoi(token);
			result += value;
		}

		no = sprintf(buff, "The answer is: %.2f \n", result);
		write(STDOUT_FILENO, buff, no);
	}
}
