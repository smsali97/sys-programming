/*
 * labtask_26.c
 *
 *  Created on: Jan 26, 2019
 *      Author: sualeh
 */
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


int main4(int argc, char **argv) {

	const int SIZE = 10;
	char buff[SIZE];

	while (true) {
		// picks number of bytes from buffer and reads it
		int no = read(STDIN_FILENO,buff,SIZE);
		//x = write(STDOUT_FILENO, buff, no);

		int no1, no2, result;

		sscanf(buff,"%d %d",&no1,&no2);
		result=no1+no2;
	    int count = sprintf(buff,"%i",result);
	    write(STDOUT_FILENO,buff,count);

	}

}
