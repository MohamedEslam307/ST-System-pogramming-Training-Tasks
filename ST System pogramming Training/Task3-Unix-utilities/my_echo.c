
#include <stdio.h>

int main(int argc, char **argv)
{
    if (1 == argc) {
	printf("\n");
	return 0;
    } else {
	int arg_index = 1;
	for (arg_index = 1; arg_index < argc; arg_index++) {
	    printf("%s ", argv[arg_index]);
	}
	printf("\n");
	return 0;

    }
}
