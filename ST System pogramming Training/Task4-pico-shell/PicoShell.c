#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

#define MAX_PATH_SIZE	100

/*this function transform the every word from input string stream into 
string in the 2-d array*/
char **parse_line(char *string, int *arguments_num)
{
    char **parsed_string = NULL;
    int token_size = 0;
    int string_size = strlen(string);
    int arr_index = 0;
    *arguments_num = 1;
    //remove the spaces before the command line
    while ((arr_index < string_size) && (string[arr_index] == ' ')) {
	arr_index++;
	//printf("Number of spaces =%d\n", arr_index);
    }
    //remove the spaces after the command line
    while ((string_size > 0) && (string[string_size - 1] == ' ')) {
	// printf("string size =%d\n", string_size);
	string_size--;
    }

    //printf("String size =%d\n", string_size);
    //this loop counts the number of arguments and skip spaces
    while (arr_index < string_size) {
	if (string[arr_index] == ' ') {	//skip spaces if the user pressed between arguments
	    if ((string[arr_index + 1] == ' ')) {
		arr_index++;
		continue;
	    }
	    (*arguments_num)++;
	}
	arr_index++;
    }
    //printf("Number of arguments =%d\n", *arguments_num);
    char *token = strtok(string, " ");	//make token points to the frist word
    if (token == NULL) {	//there was no string
	*arguments_num = 0;
	parsed_string = NULL;
    } else {
	parsed_string = (char **) malloc(*arguments_num * sizeof(char *));	//allocate memory space by number of arguments
	for (arr_index = 0; arr_index < *arguments_num; arr_index++) {
	    token_size = strlen(token);	//calculate the size of every argument
	    parsed_string[arr_index] = (char *) malloc(token_size * sizeof(char));	//allocate memory space for every argument
	    strcpy(parsed_string[arr_index], token);	//copy a argument into an array elemnt
	    token = strtok(NULL, " ");	//make token points to the next word
	    /*while(token[0]==' '){
	       token = strtok(NULL, " ");
	       } */
	}
    }
    //(*arguments_num)--;
    for (int i = 0; i < *arguments_num; i++) {
	//printf("%s\n", parsed_string[i]);
    }
    // printf("Number of arguments =%d\n", *arguments_num);
    return parsed_string;
}

int main()
{
    char *command_line;
    char **parsed_input_string;
    int size_input_string = 0;
    int arguments_size = 0;
    unsigned long n = 0;
    linkedlist list;
    list.listSize = 0;
    list.head = NULL;
    printf("Welcome To My Shell \n");

    while (1) {
	printf(">>>");

	size_input_string = getline(&command_line, &n, stdin);	// get the command line from user

	if ((command_line[0] == '\n')) {	//|| (command_line[size_input_string-2] == ' ')) {
	    continue;
	}
        
	command_line[strlen(command_line) - 1] = 0;	//remove '\n' from the end of the line
	parsed_input_string = parse_line(command_line, &arguments_size);	//parsing the line into 2-d array
	addNode(&list, parsed_input_string, &arguments_size);
	//printf("command_line = %s\n",command_line);
	//printf("list_head = %s\n",list.head->Data);
	//printf("size = %d , cmd = %s\n",list->listSize,AddedNode->Data);
	//free(parsed_input_string[arguments_size-1]);
	//parsed_input_string[arguments_size-1]=NULL;
	//parsed_input_string[strlen(parsed_input_string[arguments_size-1])-1]=0;
	if (parsed_input_string != NULL) {
	    if ((strcmp(parsed_input_string[0], "exit") == 0)) {
		break;
	    } else if (strcmp(command_line, "history") == 0) {
		Print(&list);
	    } else if (strcmp(parsed_input_string[0], "echo") == 0) {
		if (1 == arguments_size) {
		    printf("\n");	//if the user echo nothing it prints new line
		} else {
		    int arg_index = 1;
		    //if he prints many strings the loop prints them all
		    for (arg_index = 1; arg_index < arguments_size;
			 arg_index++) {
			printf("%s ", parsed_input_string[arg_index]);
		    }
		    printf("\n");
		}
	    } else if ((strcmp(parsed_input_string[0], "cd") == 0)) {	//||(strcmp(parsed_input_string[0], "cd\n"))) {
		if (2 == arguments_size) {
		    //chdir returns 0 when it execute correctly 
		    if (chdir(parsed_input_string[1]) != 0) {
			printf("directory can't be reatched\n");
		    }
		} else {
		    printf("wrong number of argumenrs to cd\n");
		}
	    } else if ((strcmp(parsed_input_string[0], "pwd") == 0)) {	//)||(strcmp(parsed_input_string[0], "pwd\n") == 0)) {
		char *current_path = NULL;
		/*this function returns the path as string and null if the size exceeds max path size */
		current_path = getcwd(NULL, MAX_PATH_SIZE);
		if (NULL == current_path) {
		    printf
			("size exceeds max path size current directory\n");
		} else {
		    printf("%s\n", current_path);
		    /*now freeing the array because it allocated dynamiclly */
		    free(current_path);
		}
	    } else {
		pid_t pid_ret = fork();
		if (pid_ret > 0) {
		    int wstatus;
		    wait(&wstatus);
		} else if (pid_ret == 0) {
		    execvp(parsed_input_string[0], parsed_input_string);
		    printf("ERROR: Command not found\n");
		    return -1;
		} else {
		    printf("ERROR: I could not get a child\n");
		}
	    }

	    free(command_line);
	    command_line = NULL;
	    for (int i = 0; i < arguments_size; i++) {
		//free(parsed_input_string[i]);
		//parsed_input_string[i] = NULL;
	    }

	}
    }
    return 0;
}
