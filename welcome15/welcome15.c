#include <stdio.h>
#include <stdlib.h>

//Available functions: https://github.com/KilianKegel/toro-C-Library#implementation-status

int main(int argc, char **argv) 
{
    printf("running \"DIR:\"\n");
	system("DIR");

	printf("running \"echo 1234567:\"\n");
	system("echo 1234567");

	printf("running \"VOL:\"\n");
	system("vol");

	printf("running \"VER:\"\n");
	system("ver");

}