/*
 * lab_3rdapril.c
 *
 *  Created on: Apr 3, 2019
 *      Author: sualeh
 */

int main(int argc, char **argv) {

	int x = fork();

	if ( x > 0) {
		wait();


	}
	else {
		return 0;
	}
}


