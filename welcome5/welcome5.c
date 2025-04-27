#include <stdio.h>

// Available functions: https://github.com/KilianKegel/toro-C-Library#implementation-status

int main(int argc, char** argv) {

    int i = 0;
    char string[] = { "Welcome, to the jungle\n" };

    printf("Simple Hello, World using putc() to stdout and stderr:\n");

    do {
        putc(string[i], stdout);    // Surprise, surprise: putc() does NOT necessarily write a character directly to STDOUT.
        putc(string[i], stderr);    // The C Standard allows 'putc()' to be implemented as a macro or a function.
        // Torito's implementation is compatible with LIBCMT.LIB.
    } while (string[i++]);          // Continue until the string terminator '\0' is encountered, incrementing i by 1.

    // 1. Observe the "funny output" behavior.
    // 2. Redirect the output of stdout to a file.
    // 3. Observe the "funny output" behavior again.
    return 0;
}

