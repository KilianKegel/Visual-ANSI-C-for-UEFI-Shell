#include <stdio.h>

//Available functions: https://github.com/KilianKegel/toro-C-Library#implementation-status

int main(int argc, char **argv) {

    int i = 0;
    char string[] = { "Welcome, to the jungle\n" };

    printf("simple Hello, world using fputc() to stdout and stderr:\n");

    do {
        fputc(string[i], stdout);   // SURPRISE, SURPRISE: fputc() DOES write a character necessarily to a given FILE*,

        fputc(string[i], stderr);   // toritoc's is compatible to LIBCMT.LIB, that means fputc() is same as putc();

    } while (string[i++]);  // do as long as the string termination ZERO '\0' not appeares AND advance i by 1

    // 1. Please notify the funny output
    // 2. redirect the output of stdout to a file
    // 3. Please notify the funny output
    return 0;
}
