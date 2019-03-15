/*
 * lab1_4thmarch.c
 *
 *  Created on: Mar 4, 2019
 *      Author: sualeh
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

int main(int argc, char** argv) {

    int pid = fork();

    if (pid > 0) {
        sleep(10);
        int inputsize2 = 10;
        char inputstr1[inputsize2];

        int fd2 = open("practice.txt", O_RDWR);
        int readinput1=1;


            readinput1 = read(fd2, inputstr1, inputsize2);

            int input1, input2;
            sscanf(inputstr1, "%d %d", &input1, &input2);

            int result = input1 + input2;
            char buff[inputsize2];
            sprintf(buff, "%d", result);
            write(STDOUT_FILENO, buff, inputsize2);



        close(fd2);

    } else if(pid==0) {
        int fd = open("practice.txt", O_RDWR|O_TRUNC);

        int inputsize = 50;
        char inputstr[inputsize];
        int readinput, writeinput;
        for (int i = 0; i < 10; i = i + 1) {
            readinput = read(STDIN_FILENO, inputstr, inputsize);
            writeinput = write(fd, inputstr, readinput);
        }
        close(fd);
    }



    return (EXIT_SUCCESS);
}


