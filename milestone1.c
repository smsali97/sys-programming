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

int main(void) {

	const int SIZE = 30;
	char buff[SIZE];
	bool errFlag = false;

	while (true) {
		// picks number of bytes from buffer and reads it
		int no = read(STDIN_FILENO, buff, SIZE);

		// Error handling
		if (no == -1) {
			perror("READ ERROR: ");
			exit(1);
		}

		// To take input
		char str[no - 1];
		// Delimiter for token breaking
		const char delimiter[1] = " ";
		char* token;
		sscanf(buff, "%[^\n]", str);

		// Get first token
		token = strtok(str, delimiter);

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
				no = sprintf(buff,
						"There was a non-digit character in your input\n");
				write(STDOUT_FILENO, buff, no);
			}

			// perform relevant operation
			value = atoi(token);
			result += value;
		}

		if (!errFlag) {

			no = sprintf(buff, "The answer is: %.2f \n", result);
			write(STDOUT_FILENO, buff, no);
		}
	}
}

