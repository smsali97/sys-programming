/*
 *
 * client.c
 *
 *  Created on: Apr 14, 2019
 *      Author: sualeh
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define MY_PORT 8000
#define BUF_SIZE 1024
int main(int argc, char **argv) {
    char buf[BUF_SIZE];
    
    int sock_fd = socket(AF_INET,SOCK_STREAM,0);
    if (sock_fd == -1) {
        perror("socket: ");
        exit(1);
    }
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(MY_PORT);
    server_addr.sin_family = AF_INET;

    int ret = connect(sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (ret == -1) {
        perror("connect: ");
        exit(1);
    }

    while (true) {
        // get input from stdin
        int no = read(STDIN_FILENO, buf, BUF_SIZE);
        if (no == -1) {
            perror("read: ");
            exit(1);
        }
        // write it to the server
        no = write(sock_fd, buf, no);
        if (no == -1) {
            perror("write: ");
            exit(1);
        }
        // receive output from server
            no = read(sock_fd, buf, no);
            if (no == -1 ) {
                perror("read: ");
            }
            else if (no == 0) break;
            else {
                // then write it back
                no = write(STDOUT_FILENO,buf,no);
                if (no == -1) {
                    perror("write: ");
                }
            }
        

    }
    

}
