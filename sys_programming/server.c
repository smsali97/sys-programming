/*
 * server.c
 *
 *  Created on: Apr 5, 2019
 *      Author: sualeh
 */
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <wait.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <poll.h>
#include <arpa/inet.h>

#define MY_PORT 8000
#define BUF_SIZE 1024
#define Q_LEN 20
const int SIZE = BUF_SIZE;

typedef struct my_process {
	int pid;
	char * name;
	bool isActive;
	bool isValid;
	time_t startingTime;
	time_t endingTime;

} my_process;

#define MAX_LIMIT 100
int ctr = 0;
// Create an array of processes
my_process process_arr[MAX_LIMIT];

// Delimiter for token breaking
#define delimiter " "

// prototypes of my function
void my_arithmetic(int sign, char* token);
void my_run(char* token);
void my_list(char* token);
void my_kill(char* token);
void my_help();
void my_signal_handler(int sig_no);

void my_clist(char* token);
void my_print(char* token);
void my_cexit(char* token);

// Number of bytes read/written
int no, ret, sock_fd;

// Struct for storing peer addresses
typedef struct my_client {
	struct sockaddr_in peerSocket;
	bool isActive;
	time_t startingTime;
	time_t endingTime;
	int sock_fd;
} my_client;

my_client my_clients[Q_LEN];
int peer_ctr = 0;

int main(int argc, char **argv) {
	char buff[BUF_SIZE];

	// list of client commands
	const char add[] = "add";
	const char run[] = "run";
	const char quit[] = "exit";
	const char list[] = "list";
	const char help[] = "help";
	const char sub[] = "sub";
	const char mul[] = "mul";
	const char div[] = "div";
	const char kill[] = "kill";

	// list of server commands
	const char print[] = "print";
	const char cexit[] = "cexit";
	const char clist[] = "clist";

	// signal handling
	signal(SIGCHLD, &my_signal_handler); // if child unexpectedly terminates

	// Struct For multiplexing socket fds
	struct pollfd pfds[2];
	// stdin
	pfds[0].fd = STDIN_FILENO;
	pfds[0].events = POLLIN;

	int ret;
	int osock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (osock_fd == -1) {
		perror("socket: ");
		exit(1);
	}

	// structure for Internet sock address
	struct sockaddr_in my_server;

	my_server.sin_family = AF_INET;
	// binding for ip address
	my_server.sin_addr.s_addr = htonl(INADDR_ANY);
	// convert to network order
//	my_server.sin_port = htons(MY_PORT);

	// bind socket with port and ip
	ret = bind(osock_fd, (struct sockaddr *) &my_server, sizeof(my_server));
	if (ret == -1) {
		perror("bind: ");
		exit(1);
	}

	// start listening
	ret = listen(osock_fd, Q_LEN);
	if (ret == -1) {
		perror("listen: ");
		exit(1);
	}

	no = sprintf(buff, "Connected on %s on %d port....\n",
			inet_ntoa(my_server.sin_addr), ntohs(my_server.sin_port));
	no = write(STDOUT_FILENO, buff, no);

	pfds[1].fd = osock_fd;
	pfds[1].events = POLLIN;

	// ignore unused fds
//	for (int i = 2; i < Q_LEN; i++) {
//		pfds[i].fd = -1;
//	}

	int len = sizeof(my_clients[peer_ctr].peerSocket);
	while (true) {
		int pstatus = poll(pfds, 2, -1);
		if (pstatus < 0) {
			perror("poll: ");
		}
		if (pstatus > 0) {
			if (pfds[1].revents & POLLIN) {
				int no;
				if (peer_ctr == Q_LEN) {
					no = sprintf(buff, "PEERS LIMIT EXCEEDED!\n");
					no = write(STDOUT_FILENO, buff, no);
					exit(0);
				}
				// waiting to accept connections
				sock_fd = accept(osock_fd,
						(struct sockaddr *) &my_clients[peer_ctr].peerSocket,
						&(len));
				if (sock_fd == -1) {
					perror("accept");
					exit(1);
				}
				my_clients[peer_ctr].sock_fd = sock_fd;
				my_clients[peer_ctr].isActive = true;
				time(&my_clients[peer_ctr++].startingTime);
				int pid_child = fork();
				if (pid_child > 0)
					continue;
				else {
					ctr = 0; // initialize running processes to zero
					// child connection
					while (true) {
						no = read(sock_fd, buff, BUF_SIZE);
						if (no == -1) {
							perror("read: ");
							exit(1);
						}
						if (no == 0) {
							exit(0);
						}

						char str[no - 1];
						sscanf(buff, "%[^\n]", str);

						// Get first token
						char *token = strtok(str, delimiter);

						// Evaluate token
						if (strcmp(token, add) == 0) {
							my_arithmetic(1, token);
						} else if (strcmp(token, sub) == 0) {
							my_arithmetic(2, token);
						} else if (strcmp(token, mul) == 0) {
							my_arithmetic(3, token);
						} else if (strcmp(token, div) == 0) {
							my_arithmetic(4, token);
						} else if (strcmp(token, run) == 0) {
							my_run(token);
						} else if (strcmp(token, list) == 0) {
							my_list(token);
						} else if (strcmp(token, kill) == 0) {
							my_kill(token);
						} else if (strcmp(token, quit) == 0) {
							no = sprintf(buff, "Bye!\n");
							close(sock_fd);
							write(sock_fd, buff, no);
							exit(0);
						} else if (strcmp(token, help) == 0) {
							my_help();
						} else {
							no = sprintf(buff, "Invalid Command \n");
							write(sock_fd, buff, no);
						}
					}
				}

			}
			// read data from stdin
			else if (pfds[0].revents & POLLIN) {
				int no = read(STDIN_FILENO, buff, SIZE);

				if (no == 0) {
					no = sprintf(buff, "Invalid Command \n");
					write(STDOUT_FILENO, buff, no);
					continue;
				}
				char str[no - 1];
				sscanf(buff, "%[^\n]", str);
				// Get first token
				char *token = strtok(str, delimiter);

				// Evaluate token
				if (strcmp(token, clist) == 0) {
					my_clist(token);
				} else if (strcmp(token, print) == 0) {
					my_print(token);
				} else if (strcmp(token, cexit) == 0) {
					my_cexit(token);
				} else {
					no = sprintf(buff, "Invalid Command \n");
					write(STDOUT_FILENO, buff, no);
				}

			}
		}
	}

}

void my_cexit(char* token) {
	char buf[SIZE];
	char* ip = strtok(NULL, delimiter);
	if (ip == NULL) {
		for (int i = 0; i < peer_ctr && my_clients[i].isActive; i++) {
			no = sprintf(buf, "exit");
			no = write(my_clients[i].sock_fd, buf, no);
		}
	}

}

void my_print(char* token) {
	char buf[SIZE];
	char* printMessage = strtok(NULL, delimiter);
	char* ip = strtok(NULL, delimiter);

	if (printMessage == NULL) {
		int no = sprintf(buf, "No message provided!");
		no = write(STDOUT_FILENO, buf, no);
		return;
	}
	if (ip == NULL) {
		for (int i = 0; i < peer_ctr && my_clients[i].isActive; i++) {
			no = sprintf(buf, "message: %s\n", printMessage);
			no = write(my_clients[i].sock_fd, buf, no);
		}
	} else {
		char *temp;
		for (int i = 0; i < peer_ctr && my_clients[i].isActive; i++) {
			inet_ntop(AF_INET, &(my_clients[i].peerSocket.sin_addr), temp,
					INET_ADDRSTRLEN);
			if (strcmp(temp, ip) == 0) {
				no = sprintf(buf, "print message: %s\n", printMessage);
				int no2 = write(STDOUT_FILENO, buf, no);
				no = write(my_clients[i].sock_fd, buf, no2);
			}
		}
	}
}

void my_clist(char* token) {
	char buff[SIZE];

	no = sprintf(buff, "Total clients so far: %d\n", peer_ctr);
	no += sprintf(buff + no, "    I.P   \tactive\tStarting Time\n");
	for (int i = 0; i < peer_ctr; i++) {
		char s[9];
		strftime(s, 9, "%X", gmtime(&process_arr[i].startingTime));

		no += sprintf(buff + no, "%10s \t %s \t %8s \n",
				inet_ntoa(my_clients[i].peerSocket.sin_addr),
				my_clients[i].isActive ?
						"\x1B[32mtrue\x1B[0m" : "\x1B[31mfalse\x1B[0m", s);

	}
	no = write(STDOUT_FILENO, buff, no);
	return;

}

void my_arithmetic(int sign, char* token) {
	char buff[SIZE];

	// Float to store result with temporary number;
	token = strtok(NULL, delimiter);
	if (token == NULL)
		return;

	for (int i = 0; token[i] != '\0'; i++) {
		if (!isdigit(token[i])) {
			no = sprintf(buff,
					"There was a non-digit character '%c' in your input\n",
					token[i]);
			write(sock_fd, buff, no);
			return;
		}
	}
	double result = atof(token);
	double value;

	// Get Next Tokens
	while ((token = strtok(NULL, delimiter)) != NULL) {

		// Input Checking
		for (int i = 0; token[i] != '\0'; i++) {
			if (!isdigit(token[i])) {
				no = sprintf(buff,
						"There was a non-digit character '%c' in your input\n",
						token[i]);
				write(sock_fd, buff, no);
				return;
			}
		}

		// perform relevant operation
		value = atoi(token);

		switch (sign) {
		case 1:
			result += value;
			break;
		case 2:
			result -= value;
			break;
		case 3:
			result *= value;
			break;
		case 4:
			if (value == 0) {
				no =
						sprintf(buff,
								"Tch. You're bad at maths. This is division by zero! \n");
				write(sock_fd, buff, no);
				return;
			}
			result /= value;
			break;
		default:
			break;
		}
	}

	no = sprintf(buff, "The answer is: %.2f \n", result);
	write(STDOUT_FILENO, buff, no);

	no = sprintf(buff, "The answer is: %.2f \n", result);
	write(sock_fd, buff, no);

}

void my_run(char* token) {

	char buff[SIZE];

	if (ctr == MAX_LIMIT) {
		no =
				sprintf(buff,
						"I'm sorry my process table is full. I cannot create anymore processes!\n");
		write(sock_fd, buff, no);
		return;
	}

	char* next_token = strtok(NULL, " ");
	if (next_token == NULL) {
		no = sprintf(buff, "No argument provided! \n");
		write(sock_fd, buff, no);
		return;
	}

	// Create a pipe

	int my_pipe[2];
	pipe2(my_pipe, O_CLOEXEC);

	int my_pid = fork();
	if (my_pid == -1) {
		perror("FORK FAILED");
		return;
	}
	// child process
	else if (my_pid == 0) {
		close(my_pipe[0]);

//		no = sprintf(buff, "I'm a child with name %s! My id is %d\n My parent's id is %d\n",
//				next_token, getpid(), getppid());
//		write(sock_fd, buff, no);

		no = execlp(next_token, NULL);
		if (no == -1) {
			if (errno == ENOENT) {
				no = execl(next_token, NULL);
				if (no == -1) {
					perror("EXECL FAILED TOO: ");
					// delete the entry
					no = sprintf(buff, "FAILED");
					no = write(my_pipe[1], buff, no);
					return;
				}
			} else {
				perror("EXECLP FAILED");
				// delete the entry
				return;
			}
		}

	}
	// parent process
	else {
		close(my_pipe[1]);

//		no = sprintf(buff, "I'm a parent with id %d! \n", getpid());
//		write(sock_fd, buff, no);

		int created_pid;

		no = read(my_pipe[0], buff, SIZE);
		if (no > 0) {
			no = sprintf(buff, "Exec failed\n");
			no = write(sock_fd, buff, no);
			return;
		}

		process_arr[ctr].pid = my_pid;
		process_arr[ctr].isActive = true;
		process_arr[ctr].name = strdup(next_token);
		time(&process_arr[ctr++].startingTime);

		close(my_pipe[1]);
		no = sprintf(buff, "Run successful\n");
		no = write(sock_fd, buff, no);
	}

}

void my_list(char* token) {
	char buff[SIZE];

	bool allProccesses = false;

	char* next_token = strtok(NULL, delimiter);
	if (next_token != NULL && strcmp(next_token, "all") == 0) {
		allProccesses = true;
	}

	no = sprintf(buff, "Actual total processes so far: %d\n", ctr);
	no +=
			sprintf(buff + no,
					"    Name   \t pid \tactive\tStarting Time \t Ending Time\t Elapsed Time\n");

	for (int i = 0; i < ctr; i++) {

		if (!allProccesses && !process_arr[i].isActive)
			continue;

		float diff = difftime(process_arr[i].endingTime,
				process_arr[i].startingTime);
		if (diff < 0) {
			time_t currTime;
			time(&currTime);
			diff = difftime(currTime, process_arr[i].startingTime);

		}

		char s[9], e[9];
		strftime(s, 9, "%X", gmtime(&process_arr[i].startingTime));
		strftime(e, 9, "%X", gmtime(&process_arr[i].endingTime));

		no += sprintf(buff + no, "%8s \t %lu\t%s\t %s \t %9s \t %f \n",
				process_arr[i].name, process_arr[i].pid,
				process_arr[i].isActive ?
						"\x1B[32mtrue\x1B[0m" : "\x1B[31mfalse\x1B[0m", s,
				process_arr[i].isActive ? "--:--:--" : e, diff);

	}
	no = write(sock_fd, buff, no);
	return;

}

void my_kill(char* token) {
	char buff[SIZE];

	char* next_token = strtok(NULL, " ");
	if (next_token == NULL) {
		no = sprintf(buff, "No argument provided to kill! \n");
		write(sock_fd, buff, no);
		return;
	}

	bool isPid = true;
	for (int i = 0; next_token[i] != '\0'; i++) {
		if (!isdigit(next_token[i])) {
			isPid = false;
			break;
		}
	}

	if (isPid) {
		int k_pid = atoi(next_token);
		no = kill(k_pid, 0);
		if (no == -1) {
			if (errno == ESRCH) {
				no = sprintf(buff, "Invalid pid! \n");
				write(sock_fd, buff, no);
				return;
			} else if (errno == EPERM) {
				no = sprintf(buff, "Not allowed to kill! \n");
				write(sock_fd, buff, no);
				return;
			}
		}

		for (int i = 0; i < ctr; i++) {
			if (k_pid == process_arr[i].pid) {
				no = kill(process_arr[i].pid, SIGTERM);
				if (no == -1) {
					perror("kill");
				}
				process_arr[i].isActive = false;
				time(&process_arr[i].endingTime);
				no = sprintf(buff, "Killed successfully!\n");
				write(sock_fd, buff, no);
				return;
			}
		}

		no = sprintf(buff, "Couldnt find your pid!\n");
		write(sock_fd, buff, no);
		return;
	} else {
		for (int i = 0; i < ctr; i++) {
			if (strcmp(next_token, process_arr[i].name) == 0) {

				no = kill(process_arr[i].pid, SIGTERM);
				if (no == -1) {
					perror("kill");
				}
				process_arr[i].isActive = false;
				time(&process_arr[i].endingTime);
				no = sprintf(buff, "Killed successfully!\n");
				write(sock_fd, buff, no);
				return;
			}
		}
		no = sprintf(buff, "Couldn't find your process name!\n");
		write(sock_fd, buff, no);
	}
}

void my_signal_handler(int sig_no) {

	char buf[SIZE];
	int no = sprintf(buf, "Signal occurred\n", no);
	no = write(STDOUT_FILENO, buf, no);

	if (sig_no == SIGCHLD) {
		for (size_t i = 0; i < ctr; i++) {
			if (process_arr[i].isActive
					&& waitpid(process_arr[i].pid, NULL, WNOHANG) > 0) {
				process_arr[i].isActive = false;
				time(&process_arr[i].endingTime);
			}
		}
	}

}

void my_help() {
	char buff[10000];
	int no =
			sprintf(buff,
					"list [all] \t lists running processes for that client. If 'all' is followed it lists all the processes for that client\nrun <process_name> \t runs the process specified by 'process_name'\nkill <pid/process_name> Kills the process specified by the 'pid' or 'process_name'\nadd <list of integers>\t");
	int no2 =
			sprintf(buff + no,
					" Adds list of integers for example n1 n2 n3 (delimited by space)\nsub <list of integers> \t Subtract list of integers for example n1 n2 n3 (delimited by space)\nmul <list of integers> \t Multiplies list of integers for example n1 n2 n3 (delimited by space)\ndiv <list of integers> \t Divides list of integers for example n1 n2 n3 (delimited by space)\nexit \t Disconnects from server and exits\n");
	no = write(sock_fd, buff, no + no2);

	if (no == -1) {
		perror("write");
		exit(1);
	}
}
