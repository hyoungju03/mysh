#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

int main() {

    // printf("The longest number of character input for stdin is: %d bytes.\n", LINE_MAX);
    char input[LINE_MAX];

    while (true) {
        // read user input from stdin
        // store it in a string buffer
        // - how to manage their size, in case of long input?
        //      - start with static sized string buffer
        printf("mysh> ");
        fgets(input, LINE_MAX, stdin);

        // with the input string, probably have to parse it into commands and arguments?
        // need string manipulation
        char *prog = strtok(input, " ");
        if (prog != NULL) {
            // printf("Run the following program: %s\n", prog);
        }
        const char *args[] = {prog, NULL};

        pid_t pid = fork();
        switch (pid) {
            case -1:
                perror("fork failed\n");
                // if fork fails, don't know what to do next
            case 0:
                // printf("This is child with pid %d\n", getpid());
                execvp(prog, args);
                exit(0);
            default:
                wait(NULL);
                // printf("This is parent with pid %d\n", pid);
        }


    }

    return 0;
}