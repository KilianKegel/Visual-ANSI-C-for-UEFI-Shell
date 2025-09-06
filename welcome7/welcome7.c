#include <stdio.h>

// Available functions: https://github.com/KilianKegel/toro-C-Library?tab=readme-ov-file#implementation-status--cross-matrix

int main(int argc, char** argv) {
    size_t size = sizeof("Welcome, to the jungle\n") - 1;

    printf("Simple Hello, World using printf(), fprintf(), fwrite(), puts() and fputs() to stdout:\n");

    printf(/*stdout*/     "Welcome, to the jungle\n");          // stdout is implied, newline must be explicitly included
    fprintf(stdout, "Welcome, to the jungle\n");                // stdout is enforced, newline must be explicitly included
    fwrite("Welcome, to the jungle\n", size, 1, stdout);        // stdout is enforced, writes raw data, no newline added
    puts("Welcome, to the jungle\n");                           // Automatically appends a newline
    fputs("Welcome, to the jungle\n", stdout);                  // stdout is enforced, newline must be explicitly included
    fwrite("###########################\n", 8, 1, stdout);      // stdout is enforced, writes raw data, no newline added

    return 0;

    // 1. What is different with puts()?
    //    - puts() automatically appends a newline to the string.
}