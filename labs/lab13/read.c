#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main(int argc, char *argv[]) {

    int key = 1234;

    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);; // TODO: get shared memory id
    char *buf =(char*)shmat(shmid, NULL, 0); // TODO: attach to shared memory
    
    while (1) {
	// TODO: print shared memory, break if "exit" is reveived
        if(strcmp(buf,"exit\n")==0){break;}
	else{
	printf("Shared Buffer: %s",buf);
        sleep(1);
	}
    }

    shmdt(buf);
    
    return 0;
}
