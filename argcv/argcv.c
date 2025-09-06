#include <stdio.h>

// Available functions: https://github.com/KilianKegel/toro-C-Library?tab=readme-ov-file#implementation-status--cross-matrix

int main(int argc, char** argv) {
    int i;

    printf("Demonstration of argument count and vector capabilities of the Standard C Library\n");
    for (i = 0; i < argc; i++)
        printf("|%s", argv[i]);
    printf("|\nArgument count is: %d\n", argc);
}
