/*
 * client.c
 *
 *  Created on: Apr 4, 2019
 *      Author: sualeh
 */

#include <ctype.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

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
const char delimiter[1] = " ";
char* token;

// prototypes of my function
void my_arithmetic(int sign);
void my_run();
void my_list();
void my_kill();
void signal_handler(int sig_no);

// Number of bytes read/written
int no, ret;
// buffer size
const int SIZE = 1000;

int main(int argc, char *argv[]) {
	char buff[SIZE];

	const char add[] = "add";
	const char run[] = "run";
	const char quit[] = "exit";
	const char list[] = "list";
	const char help[] = "help";
	const char sub[] = "sub";
	const char mul[] = "mul";
	const char div[] = "div";
	const char kill[] = "kill";

	while (true) {
		no = read(STDIN_FILENO, buff, SIZE);

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

void my_arithmetic(int sign) {
	char buff[SIZE];

	// Float to store result with temporary number;
	token = strtok(NULL, delimiter);
	for (int i = 0; token[i] != '\0'; i++) {
		if (!isdigit(token[i])) {
			no = sprintf(buff,
					"There was a non-digit character '%c' in your input\n",
					token[i]);
			write(STDOUT_FILENO, buff, no);
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
				write(STDOUT_FILENO, buff, no);
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
				write(STDOUT_FILENO, buff, no);
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

}

void my_run() {
	char buff[SIZE];

	if (ctr == MAX_LIMIT) {
		no =
				sprintf(buff,
						"I'm sorry my process table is full. I cannot create anymore processes!\n");
		write(STDOUT_FILENO, buff, no);
		return;
	}

	char* next_token = strtok(NULL, delimiter);
	if (next_token == NULL) {
		no = sprintf(buff, "No argument provided! \n");
		write(STDOUT_FILENO, buff, no);
		return;
	}

	// Create a pipe
	int my_pipe[2];
	pipe(my_pipe);

	int my_pid = fork();
	if (my_pid == -1) {
		perror("FORK FAILED");
		return;
	}
	// child process
	else if (my_pid == 0) {
		close(my_pipe[0]);

		no = sprintf(buff, "I'm a child! My id is %d\n My parent's id is %d\n",
				getpid(), getppid());
		write(STDOUT_FILENO, buff, no);

		// send data
		no = sprintf(buff, "%d", getpid());
		no = write(my_pipe[1], buff, SIZE);

		no = execlp(next_token, NULL);
		if (no == -1) {
			if (errno == ENOENT) {
				no = execl(next_token, NULL);
				if (no == -1) {
					perror("EXECL FAILED TOO: ");
					ctr--; // delete the entry
					return;
				}
			} else {
				perror("EXECLP FAILED");
				ctr--; // delete the entry
				return;
			}
		}

	}
	// parent process
	else {
		close(my_pipe[1]);

		no = sprintf(buff, "I'm a parent with id %d! \n", getpid());
		write(STDOUT_FILENO, buff, no);

		int created_pid;

		no = read(my_pipe[0], buff, SIZE);
		no = sscanf(buff, "%d", &created_pid);

		process_arr[ctr].pid = created_pid;
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
	no = write(STDOUT_FILENO, buff, no);

	no =
			sprintf(buff,
					"    Name   \t pid \tactive\tStarting Time \t Ending Time\t Elapsed Time\n");
	no = write(STDOUT_FILENO, buff, no);

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

		no = sprintf(buff, "%8s \t %lu\t%s\t %s \t %9s \t %f \n",
				process_arr[i].name, process_arr[i].pid,
				process_arr[i].isActive ? "\x1B[32mtrue\x1B[0m" : "\x1B[31mfalse\x1B[0m", s,
				process_arr[i].isActive ? "--:--:--" : e, diff);
		no = write(STDOUT_FILENO, buff, no);
	}

	return;

}

void my_kill() {
	char buff[SIZE];

	char* next_token = strtok(NULL, delimiter);
	if (next_token == NULL) {
		no = sprintf(buff, "No argument provided to kill! \n");
		write(STDOUT_FILENO, buff, no);
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
				write(STDOUT_FILENO, buff, no);
				return;
			} else if (errno == EPERM) {
				no = sprintf(buff, "Not allowed to kill! \n");
				write(STDOUT_FILENO, buff, no);
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
		write(STDOUT_FILENO, buff, no);
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
		write(STDOUT_FILENO, buff, no);
	}
}
