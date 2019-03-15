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

	/*
	 * P1: ABCDXYZ
	 * P2: ABCDXYZABCD (before O_TRUNC)
	 * P2: 123LMN00000XYZ (after O_TRUNC)
	 */

	int f1 = open("abc",O_CREAT | O_RDWR,S_IRWXU);
	int f2;

	int c = fork();
	write(f1, "ABCD", 4);

	if ( c == 0) {
		f2 = open("abc",O_CREAT | O_RDWR | O_TRUNC,S_IRWXU);
		write(f2,"123",3);

	}
	write(f1,"XYZ",3);
	if ( c == 0) {
		write(f2,"LMN",3);
	}
}

