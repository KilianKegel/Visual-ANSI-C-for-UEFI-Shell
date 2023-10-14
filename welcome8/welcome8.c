#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Available functions: https://github.com/KilianKegel/toro-C-Library#implementation-status

#define TTYSIZE 80

char welcomelyrics[] = {
    "Welcome to the Jungle\n"
    "=====================\n"
    "Welcome to the jungle we've got fun and games\n"
    "We got everything you want honey, we know the names\n"
    "We are the people that can find whatever you may need\n"
    "If you got the money honey we got your disease\n"
    "\r\n"
    "In the jungle, welcome to the jungle\n"
    "Watch it bring you to your knnn knne knees, knees\n"
    "I want to watch you bleed\n"
    "\r\n"
    "Welcome to the jungle we take it day by day\n"
    "If you want it you're gonna bleed but it's the price to pay\n"
    "And you're a very sexy girl that's very hard to please\n"
    "You can taste the bright lights but you won't get there for free\n"
    "In the jungle welcome to the jungle\n"
    "Feel my, my, my serpentine\n"
    "Ooh, I want to hear you scream\n"
    "\r\n"
    "Welcome to the jungle it gets worse here every day\n"
    "Ya learn to live like an animal in the jungle where we play\n"
    "If you hunger for what you see you'll take it eventually\n"
    "You can have everything you want but you better not take it from me\n"
    "\r\n"
    "In the jungle, welcome to the jungle\n"
    "Watch it bring you to your knnn knne knees, knees\n"
    "I want to watch you bleed\n"
    "\r\n"
    "And when you're high you never ever want to come down\n"
    "So down, so down, so down, yeah\n"
    "\r\n"
    "You know where you are?\n"
    "You're down in the jungle baby, you're gonna dieee\n"
    "In the jungle welcome to the jungle\n"
    "Watch it bring you to your knees, knees\n"
    "In the jungle welcome to the jungle\n"
    "Feel my, my, my serpentine\n"
    "In the jungle welcome to the jungle\n"
    "Watch it bring you to your knees, knees\n"
    "In the jungle welcome to the jungle\n"
    "Watch it bring you to you\n"
    "Its gonna bring you down, ha!!\n"
    "\r\n"
};

char *centerstring(char *string)
{
    static char buffer[TTYSIZE];
    size_t length = strlen(string);

        memset(buffer,' ',TTYSIZE);
        strcpy(&buffer[TTYSIZE - 1 - strlen("\n")],"\n");

        strncpy(&buffer[(TTYSIZE / 2) - length / 2], string, length);

    return &buffer[0];
}

int main(int argc, char **argv){
    char *pLine;
    
    pLine = strtok (welcomelyrics, "\n");

    while (pLine != NULL){
        printf ("%s" , centerstring(pLine));
        pLine = strtok (&pLine[strlen(pLine) + 1],"\n");
    }
    printf ("\n");  
}
