#include <stdio.h>
#include <signal.h>
#include <time.h>

// Available functions: https://github.com/KilianKegel/toro-C-Library?tab=readme-ov-file#implementation-status--cross-matrix

void myCtrlCHandler (int signum) {
    printf("You pressed ctrl+c, but I don't care\n");
}

int main() {
    clock_t clk;

    signal(SIGINT, myCtrlCHandler);

    while (1) {
        printf("Welcome, to the Jungle... (first CTRL-C can not break me)\n");

        clk = CLOCKS_PER_SEC / 2 + clock();

        while (clk > clock())
            ;
    }

    return 0;
}