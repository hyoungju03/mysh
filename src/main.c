#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

#define CMD_DELIM " \n"
#define BIN_DIR "/bin"


int main() {

    // printf("The longest number of character input for stdin is: %d bytes.\n", LINE_MAX);
    char input[LINE_MAX];
    const char* cmd_delim = " ";

    while (true) {
        // read user input from stdin
        // store it in a string buffer
        printf("mysh> ");
        fgets(input, LINE_MAX, stdin);

		// max number of arguments?
		const int MAX_N_ARG = 10;
		char *argv[MAX_N_ARG];

		char *token;
		int arg_count = 0;
		for (token = strtok(input, CMD_DELIM); token; token = strtok(NULL, CMD_DELIM)) {
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

		// // DEBUG: print all parsed arg elements
		// for (int i = 0; i < arg_count; i++) {
		// 	printf("arg[%d]: %s", i, argv[i]);
		// 	if (i < arg_count-1) printf(", ");
		// }
		// printf("\n");
		
		const char *file = argv[0];
		
		if (strcmp(file, "cd") == 0) {
			if (chdir(argv[1]) == -1) {
				printf("%s: no such file or directory: %s\n", file, argv[1]);
			}
			continue;
		}

        pid_t pid = fork();
        switch (pid) {
            case -1:
                perror("fork failed\n");
            case 0:
				// move child process into different process group
				// setpgid(0, 0);
				// printf("Child PID: %ld, PGID: %ld\n", (long)getpid(), (long)getpgrp());
                execvp(file, argv);
				exit(0);
            default:
				// setpgid(0, 0);
				// printf("Parent PID: %ld, PGID: %ld\n", (long)getpid(), (long)getpgrp());
                wait(NULL);
        }
    }

    return 0;
}
