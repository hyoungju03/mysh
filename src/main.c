#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>
#include <signal.h>

#include <fcntl.h>

#define CMD_DELIM " \n"


int main() {

	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGTTOU);

    char input[LINE_MAX];

	// contains current working directory
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) == NULL) {
		perror("failed to fetch current working directory...\n");
		exit(1);
	}
	char *current_path = basename(cwd);

    while (true) {
        // read user input from stdin
        // store it in a string buffer
        printf("mysh> %s ~ ", current_path);
        fgets(input, LINE_MAX, stdin);

		// max number of arguments?
		const int MAX_N_ARG = 10;
		char *argv[MAX_N_ARG];

		char *token;
		int arg_count = 0;

		// Flag for output redirection
		unsigned int out_redir = 0;
		unsigned int in_redir = 0;
		char *redir_file;

		for (token = strtok(input, CMD_DELIM); token; token = strtok(NULL, CMD_DELIM)) {
			// Look for output redirection indicator '>'
			if (strcmp(token, ">") == 0) {
				out_redir = 1;
				redir_file = strtok(NULL, CMD_DELIM);
				// printf("OUTPUT REDIRECTED TO FILE: %s\n", redir_file);
				break;
			}
			// look for input redirection indicator '<'
			if (strcmp(token, "<") == 0) {
				in_redir = 1;
				redir_file = strtok(NULL, CMD_DELIM);
				break;
			}
			argv[arg_count] = token;
			arg_count += 1;
		}

		// arg array must end with NULL pointer
		argv[arg_count] = NULL;

		// empty arg list
		if (arg_count == 0) {
			// printf("Empty command...\n");
			continue;
		}

		const char *file = argv[0];
		// printf("Run executable: %s\n", file);
		// if (arg_count > 1) {
		// 	// printf("Arguments for the executable: \n");
		// 	for (int i = 1; i < arg_count; ++i) {
		// 		printf("%s ", argv[i]);
		// 	}
		// }
		
		if (strcmp(file, "cd") == 0) {
			if (chdir(argv[1]) == -1) {
				printf("%s: no such file or directory: %s\n", file, argv[1]);
			} else {
				if (getcwd(cwd, sizeof(cwd)) == NULL) {
					perror("failed to fetch current working directory...\n");
					exit(1);
				}
				current_path = basename(cwd);
			}
			continue;
		}

		int sync_pipe[2];
		if (pipe(sync_pipe) < 0) {
			perror("pipe failed");
			exit(EXIT_FAILURE);
		}

        pid_t pid = fork();
        switch (pid) {

            case -1:
                perror("Fork failed!\n");
				exit(1);

            case 0:
				close(sync_pipe[1]);
				char dummy;
				if (read(sync_pipe[0], &dummy, 1) < 0) {
					perror("child read failed");
				}
				close(sync_pipe[0]);

				setpgid(0, 0);
				// printf("Child PID: %ld, PGID: %ld\n", (long)getpid(), (long)getpgrp());

				// Implement output redirection
				if (out_redir) {
					int redir_fd = open(redir_file, O_CREAT | O_RDWR);
					dup2(redir_fd, 1);
				}

				if (in_redir) {
					int redir_fd = open(redir_file, O_RDWR);
					dup2(redir_fd, 0);
				}
				
                execvp(file, argv);
				exit(0);

            default:
				// Close the read-end; the parent only writes to this pipe
				close(sync_pipe[0]);

				// printf("Parent PID: %ld, PGID: %ld\n", (long)getpid(), (long)getpgrp());
				pid_t child_pid = pid;
				char notification_byte = 'x';

				// move child process into a new process group
				setpgid(child_pid, child_pid);
				// change the child process group as foreground process group of the session
				if (tcsetpgrp(STDIN_FILENO, pid) < 0) {
					printf("Changing foreground process group failed.\n");
					exit(1);
				}

				// Housekeeping is complete. Write a byte to unblock the child.
				if (write(sync_pipe[1], &notification_byte, 1) < 0) {
					perror("parent write failed");
				}
				close(sync_pipe[1]);
				
                wait(NULL);
				
				sigprocmask(SIG_BLOCK, &set, NULL);
				// gain back foreground process group
				if (tcsetpgrp(STDIN_FILENO, getpgrp()) < 0) {
					printf("Restoring foreground process group failed.\n");
					exit(1);
				}
				sigprocmask(SIG_UNBLOCK, &set, NULL);
        }
    }

    return 0;
}