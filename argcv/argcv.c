#include <stdio.h>

int main(int argc, char **argv) {
	int i;
    
	printf("Demonstration of argument count and vector capability of the Standard C Library\n");
	for (i = 0; i < argc; i++)
		printf("|%s", argv[i]);
	printf("|\nArgument count is: %d\n",argc);
}