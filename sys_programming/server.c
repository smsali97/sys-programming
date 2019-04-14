/*
 * server.c
 *
 *  Created on: Apr 5, 2019
 *      Author: sualeh
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

#define MY_PORT 8000
#define BUF_SIZE 1024
#define Q_LEN 5


struct sockaddr_in peer_addresses[Q_LEN];
int peer_ctr = 0;

int main(int argc, char **argv) {
	int buf[BUF_SIZE];
	int ret;
	int sock_fd = socket(AF_INET,SOCK_STREAM, 0);
	if (sock_fd == -1) {
		perror("socket: ");
		exit(1);
	}

	// structure for Internet sock address
	struct sockaddr_in my_server;
	my_server.sin_family = AF_INET;
	// binding for localhost
	my_server.sin_addr.s_addr = INADDR_ANY;
	// convert to network order
	my_server.sin_port = htons(MY_PORT);

	ret = bind(sock_fd, (struct sockaddr *) &my_server, sizeof(my_server));
	if (ret == -1) {
		perror("bind: ");
		exit(1);
	}

	ret = listen(sock_fd,Q_LEN);
	if (ret == -1) {
		perror("listen: ");
		exit(1);
	}
	while (true) {
		int accepted_sock_fd = accept(sock_fd, (struct sockaddr *) peer_addresses[peer_ctr], sizeof(peer_addresses[peer_ctr++]));
		if (accepted_sock_fd == -1) {
			perror("accept");
			exit(1);
		}
		int pid_child = fork();
		if (pid_child > 0) continue;
		else {
			// child connection
			int no = read(accepted_sock_fd,buf,BUF_SIZE);
			if (no == -1) { perror("read: "); exit(1); }
			no = sprintf(buf,"%d",no);
			write(STDOUT_FILENO,buf,no);
		}
	}


}
