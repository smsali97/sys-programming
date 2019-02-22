/*
 * lab_09.c
 *
 *  Created on: Feb 9, 2019
 *      Author: sualeh
 */

int main20(int argc, char **argv) {
	int pid;

	int a = 20;

	pid = fork();

	printf("HI\n");

	if (pid == 0) {
		pid = fork();
		if (pid > 0)
			printf("HELLO\n");
		a = 30;
	}

	printf("HI\n");

	printf("%d\n", a);

}
