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
#define Q_LEN 3


struct sockaddr_in peer_addresses[Q_LEN];
int peer_ctr = 0;

int main(int argc, char **argv) {
	char buf[BUF_SIZE];
	int ret;
	int sock_fd = socket(AF_INET,SOCK_STREAM, 0);
	if (sock_fd == -1) {
		perror("socket: ");
		exit(1);
	}

	// structure for Internet sock address
	struct sockaddr_in my_server;
	struct sockaddr_in my_clients[peer_ctr];

	my_server.sin_family = AF_INET;
	// binding for localhost
	my_server.sin_addr.s_addr = htonl(INADDR_ANY);
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
	int len = sizeof(my_clients[peer_ctr]);
	while (true) {
		int no;
		if (peer_ctr == Q_LEN) {
			no = sprintf(buf,"PEERS LIMIT EXCEEDED!\n");
			no = write(STDOUT_FILENO,buf,no);
			exit(0);
		}

		int accepted_sock_fd = accept(sock_fd, (struct sockaddr *) &my_clients[peer_ctr++], &(len));
		if (accepted_sock_fd == -1) {
			perror("accept");
			exit(1);
		}
		int pid_child = fork();
		if (pid_child > 0) continue;
		else {
			// child connection
			while (true) {
				no = read(accepted_sock_fd,buf,BUF_SIZE);
				if (no == -1) { perror("read: "); exit(1); }
				if (no == 0) { exit(0); }
				no = sprintf(buf,"%d",no);
				no = write(STDOUT_FILENO,buf,no);
				if (no == -1) { perror("write: "); exit(1); }
			}
		}
	}


}
