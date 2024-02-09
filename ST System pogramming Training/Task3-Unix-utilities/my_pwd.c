#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PATH_SIZE	100

int main()
{
    char *current_path = NULL;
    current_path = getcwd(NULL, MAX_PATH_SIZE);
    if (NULL == current_path) {
	printf("size exceeds max path size current directory\n");
	return -1;
    } else {
	printf("%s\n", current_path);
	free(current_path);
	return 0;
    }
}
