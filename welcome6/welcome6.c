#include <stdio.h>

// Available functions: https://github.com/KilianKegel/toro-C-Library?tab=readme-ov-file#implementation-status--cross-matrix

int main(int argc, char** argv) {

    int i = 0;
    char string[] = { "Welcome, to the jungle\n" };

    printf("Simple Hello, World using fputc() to stdout and stderr:\n");

    do {
        fputc(string[i], stdout);   // fputc() writes a character to the specified FILE* (stdout in this case).
        fputc(string[i], stderr);   // fputc() writes a character to the specified FILE* (stderr in this case).
    } while (string[i++]);          // Continue until the null terminator '\0' is encountered, incrementing i by 1.

    // 1. Observe the output behavior.
    // 2. Redirect the output of stdout to a file.
    // 3. Observe the output behavior again.
    return 0;
}

