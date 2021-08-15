#include <stdio.h>

int main(int argc, char **argv) {
    printf("simple Hello, world:\n");
    printf("Welcome, to the jungle - by printf()\n");     // printf() -- print formatted, is the C library function used in the famous "hello, world"
    wprintf(L"Welcome, to the jungle - by wprintf()\n");   // wprintf() -- wprint formatted, is the C library function used in the famous "hello, world"
}