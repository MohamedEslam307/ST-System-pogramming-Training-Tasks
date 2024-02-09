#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc, char **argv)
{
    if (1 == argc) {
	printf("missing source and destination file operands\n");
	return -1;
    } else if (2 == argc) {
	printf("missing destination file operand");
	return -2;
    } else if (3 == argc) {
	    char buf[100]={0};
	   int num_write, num_read, fd_source=0,fd_dest=0;
	   fd_source= open(argv[1], O_RDONLY);
		fd_dest= open(argv[2],O_CREAT | O_WRONLY | O_TRUNC ,0644);

		if (fd_source < 0 ){
			printf("Could't open source file\n");
			return -1;
		}else if (fd_dest < 0 ){
                        printf("Could't open dest file\n");
                        return -1;
             	}
		while((num_read=read(fd_source,buf,100))!=0){
			if(num_read < 0){
				printf("could't read from the file\n");
				return -1;
			}
			num_write = write(fd_dest,buf,num_read);
			if(num_write < 0){
                                printf("could't write from the file\n");
                                return -1;
                     	}
		}
		close(fd_dest);
		close(fd_source);
		remove(argv[1]);

    } else {
	printf("Too much arguments");
	return -4;
    }
}
