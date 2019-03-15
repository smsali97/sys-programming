/*
 * lab_09.c
 *
 *  Created on: Feb 9, 2019
 *      Author: sualeh
 *
 *      Seeing patterns of His and Hellos with Fork
 */
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
	int pid;

	int a = 20;

	pid = fork();

	printf("HI\n");

	if (pid == 0) {
		pid = fork();
		// Parent
		if (pid > 0)
			printf("HELLO\n");
		a = 30;
	}

	printf("HI\n");

	printf("%d\n", a);

}
