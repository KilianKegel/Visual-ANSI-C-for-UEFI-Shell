#include <stdio.h>

//https://docs.microsoft.com/en-us/cpp/c-language/using-wmain?view=msvc-160

int wmain(int argc, wchar_t** argv) {
    int i;
    
    printf("Demonstration of argument count and vector capability of the Standard C Library in wide format\n");
    for (i = 0; i < argc; i++)
        printf("|%ls", argv[i]);
    printf("|\nArgument count is: %d\n", argc);
}