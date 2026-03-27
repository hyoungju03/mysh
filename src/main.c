#include <stdio.h>
#include <limits.h>

int main() {

    printf("The longest number of character input for stdin is: %d bytes.\n", LINE_MAX);
    char input[LINE_MAX];

    // read user input from stdin
    fgets(input, LINE_MAX, stdin);

    printf("Location of string buffer: %p\n", input);
    printf("The input string: %s\n", input);

    // store it in a string buffer
    // - how to manage their size, in case of long input?
    //      - start with static sized string buffer

    return 0;
}