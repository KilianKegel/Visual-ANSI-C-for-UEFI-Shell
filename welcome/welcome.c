#include <stdio.h>

//Available functions: https://github.com/KilianKegel/toro-C-Library#implementation-status

int main(int argc, char **argv) {
    printf("simple Hello, world:\n");
    printf("Welcome, to the jungle - by printf()\n");     // printf() -- print formatted, is the C library function used in the famous "hello, world"
    wprintf(L"Welcome, to the jungle - by wprintf()\n");   // wprintf() -- wprint formatted, is the C library function used in the famous "hello, world"
}