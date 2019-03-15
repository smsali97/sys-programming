/*
 * lab2_16thfeb.c
 *
 *  Created on: Feb 16, 2019
 *      Author: sualeh
 *
 *  Using exec to pass argument to another process (lab2_16thfeb.c)
 */

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

	char* s = argv[0];

	printf("%s \n",s);

	printf("HELLO\n");
}

