#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

void pid1func(int fd[]){
    char sendmsg[] = "I have a sample file, which consist of sample of sample";
    //scanf("%s",sendmsg);
    close(fd[0]);
    write(fd[1],&sendmsg,strlen(sendmsg)+1);
    close(fd[1]);


    //write sendmsg to the pipe
    //ensure pipe ends are close properly

}

void pid2func(int fd[]){
    // close fds as necessary
    close(fd[1]);
    int newfd=dup2(*fd,STDIN_FILENO);
    close(fd[0]);
    // redirect read end to STDIN_FILENO

    // run grep for "sample" on the text in STDIN_FILENO using execvp
    char *args[] = {"grep", "-a", "sample", NULL};
    execvp("grep", args); 
}

int main(int argc, char *argv[]){
    int fd1[2];
    

    pipe(fd1); // add error checking
    if (pipe(fd1)<0)
    {
        exit(0);
    }
    
    pid_t id1=fork();
    // How many copies of pipe is created when the following processes are created? If you do not understand this, your program may run forever.
    //which means the parent process
    if(id1==0){
        pid1func(fd1);
        
    }

    // fork first process
        // run pid1func()

    //fork second process       
        // run pid2func()


else{//parent process
    pid_t id2 = fork();
    if(id2==0)
    {
        pid2func(fd1);
        exit(0);

    }

    wait(NULL);
    close(fd1[0]);
    close(fd1[1]);
}

    // wait for both process to complete
    //  wait(NULL);
    wait(NULL);
    return 0;
}
