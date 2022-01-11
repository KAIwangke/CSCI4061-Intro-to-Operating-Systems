#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
	if (argc < 2) {

		printf("Incorrect number of args: given %d, expected 1.\n", argc - 1);
		exit(1);
	}
	pid_t child1 = fork();
	if(child1 == 0){
		execl("/bin/echo","echo","hello world",(char*)NULL);
	}
	else{
		wait(NULL);
	}
	pid_t child2 = fork();
	if(child2 == 0){
		execl("lab4.o","./lab4.o",argv[1],(char*)NULL);
	}
	else{
		wait(NULL);
		
	}
	

	
}
