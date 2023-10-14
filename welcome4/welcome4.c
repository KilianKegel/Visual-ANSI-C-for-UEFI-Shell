#include <stdio.h>

//Available functions: https://github.com/KilianKegel/toro-C-Library#implementation-status

int main(int argc, char **argv) {
    #define STRING "Welcome, to the jungle\n"
    size_t lenght = sizeof(STRING);
    char string[] = {STRING};
    
    printf("simple Hello, world using fwrite() to stdout and stderr:\n");

    fwrite(string, lenght, 1, stdout);      //fwrite(to stdout) - file write to STDOUT is identical to write to a real file -- that equality was introduced in UNIX/C
    fwrite(string, lenght, 1, stderr);      //fwrite(to stderr) - file write to STDERR is identical to write to a real file -- that equality was introduced in UNIX/C

    // HOMEWORK:
    // 1. Please take note of the additional invisible character preceeding the second line of output
    // 2. verify, by rebuilding that welcome4.c with the original microsoft library LIBCMT.LIB, that it is excpected behaviour
    // 3. explain behaviour and fix it!
    return 0;
}