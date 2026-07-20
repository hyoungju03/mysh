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
		// empty arg list
		if (arg_count == 0) {
			// printf("Empty command...\n");
			continue;
		}
		
		const char *file = argv[0];
		

		for (int i = 0; i < arg_count; i++) {
			printf("arg[%d]: %s", i, argv[i]);
			if (i < arg_count-1) printf(", ");
		}
		printf("\n");

		// char exec_path[PATH_MAX];
		// int res = snprintf(exec_path, sizeof(exec_path), "%s/%s", BIN_DIR, exec);

		// if (res < 0) printf("Buffer overflow: executable path longer than PATH_MAX\n");
        // // printf("Run executable: %s\n", exec_path);
		// char *const args[] = {"ls", NULL};

        // pid_t pid = fork();
        // switch (pid) {
        //     case -1:
        //         perror("fork failed\n");
        //         // if fork fails, don't know what to do next
        //     case 0:
        //         // printf("This is child with pid %d\n", getpid());
        //         execvp(exec, args);
        //     default:
        //         wait(NULL);
        //         // printf("This is parent with pid %d\n", pid);
        // }
    }

    return 0;
}
