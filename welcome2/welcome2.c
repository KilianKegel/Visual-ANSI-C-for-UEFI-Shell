#include <stdio.h>

// Available functions: https://github.com/KilianKegel/toro-C-Library?tab=readme-ov-file#implementation-status--cross-matrix

int main(int argc, char** argv) {
    printf("Simple Hello, World using fprintf():\n");
    fprintf(stdout, "Welcome to the jungle - by fprintf() to stdout\n");    // fprintf(stdout, ...) - file print formatted to STDOUT is identical to printf()
    fprintf(stderr, "Welcome to the jungle - by fprintf() to stderr\n");    // fprintf(stderr, ...) - file print formatted to STDERR is identical to printf(), except that it is directed to STDERR
    // ... is just another output and redirection channel (e.g., when redirecting program output to a file with the '>' operator)
    return 0;
}

