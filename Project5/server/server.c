#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>

#define MSGSZ 256
#define size 256

int arr[26];
int activeThreads;
pthread_mutex_t active_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t arr_lock = PTHREAD_MUTEX_INITIALIZER;

void* handle_multiple(void* clients);

struct msgbuf {
   long mtype;
   char mtext[MSGSZ];
};

struct argstruct {
	int client;
};

//function to count the number of words starting with 
//each letter into the global array 
void parsing(char input[size]){  
    int c  = input[0];
    if (c>64&&c<91){
        int target = c -65;

        pthread_mutex_lock(&arr_lock);
            arr[target]=arr[target]+1;
        pthread_mutex_unlock(&arr_lock);
        

    }
    else if (c>96&&c<123){
        int target = c -97;
        pthread_mutex_lock(&arr_lock);
            arr[target]=arr[target]+1;
        pthread_mutex_unlock(&arr_lock);
    }
    else{
        printf("Out of bounds\n");
    }
}

int main(int argc, char** argv) {

   //handle the receiving things
   //muti-threading handle the different clients

   printf("Server start\n");

    if (argc != 2) {
		printf("Incorrect arg count. Enter file name and thread count\n");
		exit(EXIT_FAILURE);
	}

    int clients = atoi(argv[1]);
    pthread_t thread;
    pthread_t threads[clients];
    activeThreads = clients;
    
    //arguments for the threads
    struct argstruct* clientarr[clients];
    for (int i = 0; i< clients; i++){
    	clientarr[i] = (struct argstruct*)malloc(sizeof(struct argstruct));
        clientarr[i]->client = i + 1;
    }

    for (int i=0; i < clients; ++i) {
        if(pthread_create(&threads[i], NULL, handle_multiple, (void*)clientarr[i]) != 0){
            perror("Could not create thread\n"); 
            return 1;
        }
    }
    //join the threads
    for (int i = 0; i < clients; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("error joining thread\n");
            exit(1);
        }
    }
    
    for (int i = 0; i < clients; i++) {
        free(clientarr[i]);
    }
    pthread_exit(NULL);

    printf("The server has ended\n");
    
    return 0;

}
void* handle_multiple(void* param)
{
	
    struct argstruct* args = (struct argstruct*)param;
	int  n = sizeof(arr) / sizeof(int);
    char *out_buffer [MSGSZ];
	int client = args->client;
    struct msgbuf buf;
   
    char path[256];
   
    FILE *fd;
    char store[size];

    key_t key = 100;
    int mid;
    // TODO: get the message queue id
    mid = msgget (key, 0666 | IPC_CREAT);

	
    buf.mtype = client;
   
    while(1) {
        int reading=1;
        memset(buf.mtext,0,strlen(buf.mtext));
        msgrcv (mid, (void *)&buf, sizeof (buf), buf.mtype, 0);
        //End message has been received. Wait for remaining threads to finish before sending results.
        if(strcmp(buf.mtext, "END") == 0){
            break;
        }
        //Check to make sure we didn't grab our own message.
        else if(strcmp(buf.mtext, "ACK") == 0){            
            memset(buf.mtext,0,strlen(buf.mtext));
        
            strcpy(buf.mtext,"ACK");
            msgsnd (mid, (void *)&buf, sizeof (buf), 0);             
        }
        //The received message is a path
        else{
            memset(path,0,strlen(path));
            strcpy(path, buf.mtext);

            printf("Thread %d received %s from client.\n", client, buf.mtext);
      
            fd = fopen(path,"r");
            //Path is either invalid or relative to client
            if (fd == NULL) {
                char pathBuf[300];
                memset(pathBuf,0,strlen(pathBuf));
                sprintf(pathBuf, "../client/%s", path);
                fd = fopen(pathBuf, "r");
      	        if (fd == NULL) {
      	            perror("Invalid directory\n");
                    exit(0);
                }
            }
	        while (reading) {
	            if(fgets (store, 256, fd)!=NULL ) {
		            parsing(store);
	            }
	            else{
		            reading = 0;
	            }
	        }
            fclose(fd);

            memset(buf.mtext,0,strlen(buf.mtext));
        
            //Send acknowledgement
            strcpy(buf.mtext,"ACK");
            printf("Thread %d acknowledged %s from client.\n", client, path);
            msgsnd (mid, (void *)&buf, sizeof (buf), 0); 
        }
    }

    pthread_mutex_lock(&active_lock);
        activeThreads--;
    pthread_mutex_unlock(&active_lock);

    //Wait for all active threads to terminate before sending results.
    while(activeThreads != 0){
        //Do nothing
    }

    char result[300];
    int pos = 0;
    int letterCount = 0;
    while(n--){
        pos += sprintf(result + pos, "%d#", arr[letterCount]);
        letterCount++;
    }

    memset(buf.mtext,0,strlen(buf.mtext));
    strcpy(buf.mtext, result);
    msgsnd (mid, (void *)&buf, sizeof (buf), 0);
    printf("Thread %d has sent the final results to the client.\n", client);
}