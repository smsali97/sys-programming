/*
 * labtask_11thfeb.c
 *
 *  Created on: Feb 11, 2019
 *      Author: sualeh
 *
 *  Learning: When code gets opened in O_TRUNC mode the entire file content is zero'ed, and
 *  position pointer is made to the start.
 */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
int main(int argc, char **argv) {

	int f1 = open("abc",O_RDWR);
	int f2;

	int c = fork();
	write(f1, "ABCD", 4);

	if ( c == 0) {
		f2 = open("abc",O_RDWR | O_TRUNC);
		write(f2,"123",3);

	}
	write(f1,"XYZ",3);
	if ( c == 0) {
		write(f2,"LMN",3);
	}
}

