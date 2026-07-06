#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>


#define CMD_DELIM " "

int main() {

    // printf("The longest number of character input for stdin is: %d bytes.\n", LINE_MAX);
    char input[LINE_MAX];
    const char* cmd_delim = " ";

    while (true) {
        // read user input from stdin
        // store it in a string buffer
        // - how to manage their size, in case of long input?
        //      - start with static sized string buffer
        printf("mysh> ");
        fgets(input, LINE_MAX, stdin);

		if (strcmp(input, "\n") == 0) continue;

        // with the input string, probably have to parse it into commands and arguments?
        char *token = strtok(input, CMD_DELIM);
		const char *exec = token;

		// parse additional arguments
        while (token != NULL) {
            token = strtok(NULL, cmd_delim);
        }

        printf("Run executable: %s\n", exec);

        pid_t pid = fork();
        switch (pid) {
            case -1:
                perror("fork failed\n");
                // if fork fails, don't know what to do next
            case 0:
                // printf("This is child with pid %d\n", getpid());
                // execvp(prog, args);
                exit(0);
            default:
                wait(NULL);
                // printf("This is parent with pid %d\n", pid);
        }
    }

    return 0;
}
