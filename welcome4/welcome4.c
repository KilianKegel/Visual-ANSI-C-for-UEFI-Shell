#include <stdio.h>

// Available functions: https://github.com/KilianKegel/toro-C-Library?tab=readme-ov-file#implementation-status--cross-matrix

int main(int argc, char** argv) {
#define STRING "WELCOME, TO THE JUNGLE\n"
    size_t length = sizeof(STRING) -1 ;     // adjusted length to exclude the null terminator
    char string[] = { STRING };

    printf("Simple Hello, World using fwrite() to stdout and stderr:\n");

    fwrite(string, length, 1, stdout);      // fwrite(to stdout) - file write to STDOUT is identical to writing to a real file -- this equality was introduced in UNIX/C
    fwrite(string, length, 1, stderr);      // fwrite(to stderr) - file write to STDERR is identical to writing to a real file -- this equality was introduced in UNIX/C

    return 0;
}
