#include <stdio.h>

int main(int argc, char **argv) {

    int i = 0;
    char string[] = {"Welcome, to the jungle\n"};
    
    do {
        putc(string[i], stdout);    // surprise, surpise: putc() does NOT write a character necessarily to STDOUT,
                                    // but it can be forced to do so...
        putc(string[i], stderr);    // the Standard C standard allows 'putc()' to be implemnted as macro or function.
                                    // toritoc's is compatible to LIBCMT.LIB
    }while(string[i++]);    // do as long as the string termination ZERO '\0' not appeares AND advance i by 1

    // HOMEWORK:
    // 1. Please notify the funny output
    // 2. redirect the output of stdout to a file
    // 3. Please notify the funny output
    // 4. Explain to yourself.
    return 0;
}
