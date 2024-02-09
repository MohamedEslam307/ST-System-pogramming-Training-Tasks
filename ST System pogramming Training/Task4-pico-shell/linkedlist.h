#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
	char** Data;
	char num_of_arguments;
	struct Node* next;
}Node;

typedef struct linkedlist {
	struct Node Node;
	int listSize;
	Node* head;
}linkedlist;

void addNode(linkedlist *list,char** element,int *arguments_size);
int size(linkedlist *list);
void Print(linkedlist *list);
