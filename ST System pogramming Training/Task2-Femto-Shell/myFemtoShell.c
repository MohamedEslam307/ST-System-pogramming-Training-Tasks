#include <stdio.h>
#include <string.h>

int main(){
	char input_string[1000];
	memset(input_string,'\n',1000);
	char equality_flag=0;
	while(1)
	{
		printf("tell me a secret i will nevel reveal: ");
		scanf("%[^\n]%*c",input_string);
		equality_flag=strcmp(input_string,"exit");
		if(equality_flag){
			printf("he said: %s\n",input_string);
		}else{
			printf("Good by :)\n");
			break;
		}
	}
	return 0;
}
