#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int main(int argc, char *argv[]) {

    int key = 1234;

    int shmid = shmget(key,SHM_SIZE,IPC_CREAT | 0666); //TODO: get shared memory id
    char *buf = (char*) shmat(shmid,NULL,0); //TODO: attach shared memory

    // TODO: read from std input
    while(fgets(buf, SHM_SIZE, stdin) != NULL){
	if(strcmp(buf,"exit\n")==0){break;}
    }
    
    shmdt(buf); // detach shared memory
    shmctl(shmid, IPC_RMID, 0); // destory
    
    return 0;
}
