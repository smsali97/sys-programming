/*
 * labtask_13feb.c
 *
 *  Created on: Feb 13, 2019
 *      Author: sualeh
 *
 *   Using Exec for the first time
 */

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
	int no = execlp("gedit", (char*) NULL);
	if (no == -1 ) {
		perror("EXECLP ERROR");
	}
}
