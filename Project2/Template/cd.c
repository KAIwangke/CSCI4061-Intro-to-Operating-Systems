#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include <string.h>
void cd(char* arg){
	/*insert code here*/

  
    char s[100];
    
    // printing current working directory
    // printf("%s\n", getcwd(s, 100));
    // char *before=getcwd(s,100);
    // using the command
    chdir(arg);
    // char  *after=getcwd(s,100);

    if(chdir(arg)==-1){
        printf("There's no correct input dirctory\n");
    }
    // printing current working directory
    else 
    printf("Currently in%s\n", getcwd(s, 100));



}

int main(int argc, char** argv){

	if(argc<2){
		printf("Pass the path as an argument\n");
		return 0;
	}
	cd(argv[1]);
	return 0;
}