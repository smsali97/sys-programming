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
#include <fcntl.h>
#include <sys/stat.h>
#include <poll.h>
#include <string.h>

#define MY_PORT 8000
#define BUF_SIZE 1024
int main(int argc, char* argv[]) {
    char buf[BUF_SIZE];
    int no;
    // usage
    if (argc != 3) {
    	no = sprintf(buf,"Please enter an IP address in the dotted format and Port number\n");
    	no = write(STDOUT_FILENO,buf,no);
    	exit(1);
    }


    int fd = open("printy",O_RDONLY);
    no = read(fd,buf,BUF_SIZE);
    no = write(STDOUT_FILENO,buf,no);
    int sock_fd = socket(AF_INET,SOCK_STREAM,0);
    if (sock_fd == -1) {
        perror("socket: ");
        exit(1);
    }
    struct sockaddr_in server_addr;
    if (inet_aton(argv[1],&server_addr.sin_addr) == 0) {
    	no = sprintf(buf,"Please enter a valid IP address\n");
    	no = write(STDOUT_FILENO,buf,no);
    	exit(1);
    }
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_family = AF_INET;

    int ret = connect(sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (ret == -1) {
        perror("connect: ");
        exit(1);
    }

	struct pollfd pfds[2];
	// stdin
	pfds[0].fd = STDIN_FILENO;
	pfds[0].events = POLLIN;

	// socket
	pfds[1].fd = sock_fd;
	pfds[1].events = POLLIN;

    while (poll(pfds,2, -1) > 0) {
    	if (pfds[0].revents & POLLIN) {
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
    	}
    	else if (pfds[1].revents & POLLIN) {
			// receive output from server
            no = read(sock_fd, buf, BUF_SIZE);
            if (no == -1 ) {
                perror("read: ");
            }
            else if (no == 0) {
                no = sprintf(buf,"Disconnected from Server\n");
                no = write(STDOUT_FILENO,buf,no);
                close(sock_fd);
                exit(1);
            }
            char* check_input;

//            sscanf(buf,"%s",check_input);
//            if (strcmp(check_input,"exit") == 0) {
//            	no = sprintf(buf,"Received command from server to exit\n");
//            	no = write(STDOUT_FILENO,buf,no);
//            	break;
//            }

            if (no == 0) break;
            else {
                // then write it back
                no = write(STDOUT_FILENO,buf,no);
                if (no == -1) {
                    perror("write: ");
                }
            }
    	}
    }
    close(sock_fd); // cleanup
    no = sprintf(buf,"Exiting...\n");
    no = write(STDOUT_FILENO,buf,no);
}
