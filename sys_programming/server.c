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


#define MY_PORT 8000
#define BUF_SIZE 1024
#define Q_LEN 5
const int SIZE = 1000;


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
char* token;

// prototypes of my function
void my_arithmetic(int sign);
void my_run();
void my_list();
void my_kill();
void my_signal_handler(int sig_no);

// Number of bytes read/written
int no, ret, sock_fd;

// Struct for storing peer addresses
struct sockaddr_in peer_addresses[Q_LEN];
int peer_ctr = 0;

int main(int argc, char **argv) {
	char buff[BUF_SIZE];

	const char add[] = "add";
	const char run[] = "run";
	const char quit[] = "exit";
	const char list[] = "list";
	const char help[] = "help";
	const char sub[] = "sub";
	const char mul[] = "mul";
	const char div[] = "div";
	const char kill[] = "kill";

	// signal handling
	signal(SIGCHLD,&my_signal_handler); // if child unexpectedly terminates

	
	int ret;
    int osock_fd = socket(AF_INET,SOCK_STREAM, 0);
	if (osock_fd == -1) {
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

	ret = bind(osock_fd, (struct sockaddr *) &my_server, sizeof(my_server));
	if (ret == -1) {
		perror("bind: ");
		exit(1);
	}

	ret = listen(osock_fd,Q_LEN);
	if (ret == -1) {
		perror("listen: ");
		exit(1);
	}
	int len = sizeof(my_clients[peer_ctr]);
	while (true) {
		int no;
		if (peer_ctr == Q_LEN) {
			no = sprintf(buff,"PEERS LIMIT EXCEEDED!\n");
			no = write(STDOUT_FILENO,buff,no);
			exit(0);
		}

		sock_fd = accept(osock_fd, (struct sockaddr *) &my_clients[peer_ctr++], &(len));
		if (sock_fd == -1) {
			perror("accept");
			exit(1);
		}
		int pid_child = fork();
		if (pid_child > 0) continue;
		else {
			// child connection
			while (true) {
				no = read(sock_fd,buff,BUF_SIZE);
				if (no == -1) { perror("read: "); exit(1); }
				if (no == 0) { exit(0); }
				
				char str[no - 1];
				sscanf(buff, "%[^\n]", str);

				// Get first token
				token = strtok(str, delimiter);

				// Evaluate token
				if (strcmp(token, add) == 0) {
					my_arithmetic(1);
				} else if (strcmp(token, sub) == 0) {
					my_arithmetic(2);
				} else if (strcmp(token, mul) == 0) {
					my_arithmetic(3);
				} else if (strcmp(token, div) == 0) {
					my_arithmetic(4);
				} else if (strcmp(token, run) == 0) {
					my_run();
				} else if (strcmp(token, list) == 0) {
					my_list();
				} else if (strcmp(token, kill) == 0) {
					my_kill();
				} else if (strcmp(token, quit) == 0) {
					no = sprintf(buff, "Bye!\n");
					write(STDOUT_FILENO, buff, no);
					exit(0);
				} else {
					no = sprintf(buff, "Invalid Command \n");
					write(STDOUT_FILENO, buff, no);
				}

			}
		}
	}


}


void my_arithmetic(int sign) {
	char buff[SIZE];

	// Float to store result with temporary number;
	token = strtok(NULL, delimiter);
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

void my_run() {


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
		no = sprintf(sock_fd, "No argument provided! \n");
		write(sock_fd, buff, no);
		return;
	}


	// Create a pipe
	int my_pipe[2];
	pipe2(my_pipe,O_CLOEXEC);

	int my_pid = fork();
	if (my_pid == -1) {
		perror("FORK FAILED");
		return;
	}
	// child process
	else if (my_pid == 0) {
		close(my_pipe[0]);

		no = sprintf(buff, "I'm a child with name %s! My id is %d\n My parent's id is %d\n",
				next_token, getpid(), getppid());
		write(sock_fd, buff, no);

		no = execlp(next_token, NULL);
		if (no == -1) {
			if (errno == ENOENT) {
				no = execl(next_token, NULL);
				if (no == -1) {
					perror("EXECL FAILED TOO: ");
					// delete the entry
					no = sprintf(buff,"FAILED");
					no = write(my_pipe[1],buff,no);
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

		no = sprintf(buff, "I'm a parent with id %d! \n", getpid());
		write(sock_fd, buff, no);

		int created_pid;

		no = read(my_pipe[0], buff, SIZE);
		if (no > 0) {
			return;
		}

		process_arr[ctr].pid = my_pid;
		process_arr[ctr].isActive = true;
		process_arr[ctr].name = strdup(next_token);
		time(&process_arr[ctr++].startingTime);

		close(my_pipe[1]);
	}

}

void my_list() {
	char buff[SIZE];

	bool allProccesses = false;

	char* next_token = strtok(NULL, delimiter);
	if (next_token != NULL && strcmp(next_token,"all") == 0) {
		allProccesses = true;
	}

	no = sprintf(buff, "Total processes so far: %d\n", ctr);
	no +=
			sprintf(buff + no,
					"    Name   \t pid \tactive\tStarting Time \t Ending Time\t Elapsed Time\n");
	
	for (int i = 0; i < ctr; i++) {

		if (!allProccesses && !process_arr[i].isActive) continue;

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
				process_arr[i].isActive ? "\x1B[32mtrue\x1B[0m" : "\x1B[31mfalse\x1B[0m", s,
				process_arr[i].isActive ? "--:--:--" : e, diff);
		
	}
	no = write(sock_fd, buff, no);
	return;

}

void my_kill() {
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
			    time ( &process_arr[i].endingTime);
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
			    time ( &process_arr[i].endingTime);
				return;
			}
		}
		no = sprintf(buff, "Couldnt find your process name!\n");
		write(sock_fd, buff, no);
	}
}

void my_signal_handler (int sig_no) {

	if (sig_no == SIGCHLD) {
		for(size_t i = 0; i < ctr; i++)
		{
			if (process_arr[i].isActive && waitpid(process_arr[i].pid,NULL,WNOHANG) > 0 ) {
					process_arr[i].isActive = false;
					time ( &process_arr[i].endingTime);
			}
		}
	}
	
}