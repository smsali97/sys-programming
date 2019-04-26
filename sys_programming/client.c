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
#include <signal.h>
#include <stdbool.h>


void my_signal_handler(int sig_no);

#define MY_PORT 8000
#define BUF_SIZE 1000
int sock_fd;

bool is_connected = false;

int main(int argc, char* argv[]) {
    char buf[BUF_SIZE];
    int no;
    // usage
    if (argc != 3) {
    	no = sprintf(buf,"Please enter an IP address in the dotted format and Port number\n");
    	no = write(STDOUT_FILENO,buf,no);
    	exit(1);
    }

    no = write(STDOUT_FILENO,"    _                                  ___ _ _         _   \n",60);
    no = write(STDOUT_FILENO,"   /_\\__ __ _____ ___ ___ _ __  ___   / __| (_)___ _ _| |_ \n",60);
    no = write(STDOUT_FILENO,"  / _ \\ V  V / -_|_-</ _ \\ '  \\/ -_) | (__| | / -_) ' \\  _|\n",60);
    no = write(STDOUT_FILENO," /_/ \\_\\_/\\_/\\___/__/\\___/_|_|_\\___|  \\___|_|_\\___|_||_\\__|\n",60);
    no = write(STDOUT_FILENO," To view a list of available commands please type 'help'   \n",60);

    sock_fd = socket(AF_INET,SOCK_STREAM,0);
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

	signal(SIGINT,&my_signal_handler); // add signal
    while (poll(pfds,2, -1) > 0) {
    	if (pfds[0].revents & POLLIN) {
			// get input from stdin
			int no = read(STDIN_FILENO, buf, BUF_SIZE);
			if (no == 1) continue; // empty line
			if (no == -1) {
				perror("read: ");
				exit(1);
			}
			if (no >= BUF_SIZE) {
//				write(STDOUT_FILENO,"You've exhausted the Size of the buffer, no wait while I empty it\n",67);
//				while ( (no = read(STDIN_FILENO,buf,BUF_SIZE) != 0 ) ) {
//
//				}
				continue;
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
            if (no == 1) continue; // ignore
            else if (no == 0) {
                no = sprintf(buf,"Disconnected from Server\n");
                no = write(STDOUT_FILENO,buf,no);
                close(sock_fd);
                exit(1);
            }
            char check_input[no];
            check_input[no] = '\0';
            sscanf(buf,"%[^\n]",check_input);
            if (strcmp(check_input,"sock_disconnect") == 0) {
            	no = sprintf(buf,"Received command from server to exit\n");
            	no = write(STDOUT_FILENO,buf,no);
            	close(sock_fd);
            	exit(1);
            }
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


void my_signal_handler(int sig_no) {

	char buf[BUF_SIZE];
	int no = sprintf(buf, "Signal occurred\n", no);
	no = write(STDOUT_FILENO, buf, no);

	if (sig_no == SIGINT && sock_fd != -1 ) {
		close(sock_fd);
	    no = sprintf(buf,"Received Interrupt, exiting...\n");
	    no = write(STDOUT_FILENO,buf,no);
	    exit(1);
	}

}
