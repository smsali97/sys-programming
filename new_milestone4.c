/*
 * new_milestone4.c
 *
 *  Created on: Feb 27, 2019
 *      Author: sualeh
 *
 *      Usage: Type a list of numbers delimited by space and end it by a semi-colon
 *      The lines can be single or multiple (your choice!)
 *      For example n1 n2;n3 n4;n5 n6 n7;
 */

#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFF_SIZE 100

void my_add(char buf[], int index);

int main(int argc, char **argv) {
	char buf[BUFF_SIZE];
	const char delimiter[1] = " ";
	const char endLine = ';';
	bool foundEndLine = false;
	int index = 0;
	int noOfBytes;

	while (true) {
		// Reads no of bytes
		noOfBytes = 0;
		if (!foundEndLine) {
			noOfBytes = read(STDIN_FILENO, buf + index, BUFF_SIZE);
			if (noOfBytes == -1) {
				perror("READ: ");
			}
			buf[noOfBytes + index - 1] = ' ';
		} else {
			foundEndLine = false;
		}
		// Find end line!

		for (int i = 0; i < noOfBytes + index; i++) {
			if (buf[i] == endLine) {
				index += i;
				foundEndLine = true;
				break;
			}
		}

		if (foundEndLine) {
			while (foundEndLine) {
				my_add(buf, index);
				for (int i = index + 1; i < (index + 1) + BUFF_SIZE; i++)
					buf[i - (index + 1)] = buf[i];
				index = 0;
				foundEndLine = false;
				for (int i = 0; i < noOfBytes + index; i++) {
					if (buf[i] == endLine) {
						index += i;
						foundEndLine = true;
						break;

					}

				}
			}
		}
			else {
				index += noOfBytes;
			}
		}

	}

	void my_add(char buf[], int index) {
		int no;

		bool errFlag = false;
		char* str;
// Delimiter for token breaking
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
		while ((token = strtok(NULL, " ")) != NULL) {
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
