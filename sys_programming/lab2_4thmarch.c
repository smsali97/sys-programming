/*
 * lab2_4thmarch.c
 *
 *  Created on: Mar 4, 2019
 *      Author: Sandeep!
 *
 *      Using pipes to send a list of numbers to child and it adds them and displays
 *      to stdout
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int fd1[2];
    int fd2[2];
    pid_t p;

    if (pipe(fd1) == -1)
    {
        write(STDOUT_FILENO, "pipe failed\n", 12);
        return 1;
    }
    if (pipe(fd2) == -1)
    {
        write(STDOUT_FILENO, "pipe failed\n", 12);
        return 1;
    }
    p = fork();

    if (p < 0)
    {
        write(STDOUT_FILENO, "error\n", 6);
        return 1;
    }
    else if (p == 0)
    {
        close(fd1[0]);
        close(fd2[1]);
        char str[100];
        while (1)
        {
            read(STDIN_FILENO, str, 20);
            write(fd1[1], str, strlen(str) + 1);
            read(fd2[0], str, 100);
            write(STDOUT_FILENO, str, strlen(str));
        }
        close(fd1[1]);
        close(fd2[0]);
    }
    else
    {
        close(fd1[1]);
        close(fd2[0]);
        char bu[100];
         while (1)
         {
            int x = read(fd1[0], bu, 100);
            int sum = 0;
            for (char *strToken = strtok(bu, " "); strToken != NULL; strToken = strtok(NULL, " "))
            {
                sum = sum + atoi(strToken);
            }
            int noOfCharWrite = sprintf(bu, "%d\n", sum);
            write(fd2[1], bu, strlen(bu) + 1);
         }
        close(fd1[0]);
        close(fd2[1]);
        exit(0);
    }
}
