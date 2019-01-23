/*
 * process_error.c
 *
 *  Created on: Jan 23, 2019
 *      Author: sualeh
 */


// # <-- These are called pre-processor directives

// Why is this bad? Not debuggable / traceable
#define MAX_LENGTH 50

#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv) {

	char name[] = "HELLO";
	int size = strlen(name);
	int ret = write(3,name,size);

	if (ret == -1 ) {
		if (errno == EBADF) {
			write(STDOUT_FILENO,"error",5);
		}
	}
}


