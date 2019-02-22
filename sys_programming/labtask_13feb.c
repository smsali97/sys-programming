/*
 * labtask_13feb.c
 *
 *  Created on: Feb 13, 2019
 *      Author: sualeh
 */

#include <stdlib.h>
#include <errno.h>


int main(int argc, char **argv) {
	int no = execlp("gedit", (char*) NULL);
	if (no == -1 ) {
		perror("EXECLP ERROR");
	}
}
