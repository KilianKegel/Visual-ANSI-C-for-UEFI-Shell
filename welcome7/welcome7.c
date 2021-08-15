#include <stdio.h>

int main(int argc, char **argv) {
    size_t size = sizeof("Welcome, to the jungle\n") - 1;

    printf("simple Hello, world using printf(), fprintf(), fwrite(), puts() and fputs() to stdout:\n");

    printf  (/*stdout*/     "Welcome, to the jungle\n");                    // stdout is implied
    fprintf (stdout,        "Welcome, to the jungle\n");                    // stdout is enforced, string formatting can be done, but not needed here
    fwrite  (               "Welcome, to the jungle\n",size,1,stdout);      // stdout is enforced, strings are fixed
    puts    (               "Welcome, to the jungle\n");
    fputs   (               "Welcome, to the jungle\n",stdout);
    fwrite  (               "###########################\n", 8, 1, stdout); // stdout is enforced, strings are fixed

    return 0;

    // HOMEWORK:
    // 1. What is different with puts()?
}