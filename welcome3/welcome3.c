#include <stdio.h>

int main(int argc, char **argv) {
    fputs("Welcome, to the jungle\n",stdout);       //fputs(stdout,...) - file put string to STDOUT is identical to puts()
    fputs("Welcome, to the jungle\n",stderr);       //fputs(stderr,...) - file put string to STDERR is identical to puts(), except that is directed to STDERR
                                                    // ... is just another output and redirection channel ( e.g. when redirecting programs output to a file with the '>' operator)
    return 0;
}