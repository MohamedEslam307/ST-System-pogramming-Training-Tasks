
#include <stdlib.h>
#include<string.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#define MAX_PATH_SIZE	100

/*this function transform the every word from input string stream into 
string in the 2-d array and i assumned it's 10 strings max every string 
consists of maximum 100 chars*/
int parse_line(char *string, char parsed_string[10][100])
{
    char *token = strtok(string, " "); //make token points to the frist word
    int index = 0;
    while (token != NULL) {
	strcpy(parsed_string[index], token); //copy the frist word into the frist array elemnt
	token = strtok(NULL, " "); //make token points to the next word
	index++;
    }
    return index;
}

int main()
{
    char input_string[100];
    char parsed_input_string[10][100];
    int size = 0;
    printf("Welcome To My Shell \n");

    while (1) {
	input_string[0] = '\n';
        while (input_string[0] == '\n') {
            printf(">>> ");
            fgets(input_string, 100, stdin);
        }
        input_string[strlen(input_string) - 1] = 0;
	size = parse_line(input_string, parsed_input_string);
	if (strcmp(parsed_input_string[0], "exit") == 0) {
	    break;
	} else if (strcmp(parsed_input_string[0], "echo") == 0) {
	    if (1 == size) {
		printf("\n"); //if the user echo nothing it prints new line
	    } else {
		int arg_index = 1;
		    //if he prints many strings the loop prints them all
		for (arg_index = 1; arg_index < size; arg_index++) {
		    printf("%s ", parsed_input_string[arg_index]); 
		}
		printf("\n");
	    }
	} else if (strcmp(parsed_input_string[0], "cd") == 0) {
	    if (2 == size) {
		    //chdir returns 0 when it execute correctly 
		if (chdir(parsed_input_string[1]) != 0) {
		    printf("directory can't be reatched\n");
		}
	    } else {
		printf("wrong number of argumenrs to cd\n");
	    }
	} else if (strcmp(parsed_input_string[0], "pwd") == 0) {
	    char *current_path = NULL;
	    /*this function returns the path as string and null if the size exceeds max path size*/
	    current_path = getcwd(NULL, MAX_PATH_SIZE);
	    if (NULL == current_path) {
		printf("size exceeds max path size current directory\n");
	    } else {
		printf("%s\n", current_path);
		/*now freeing the array because it allocated dynamiclly*/
		free(current_path);
	    }
	} else {
	    printf("Error\n");
	}
	printf("\n");
    }
    return 0;
}

