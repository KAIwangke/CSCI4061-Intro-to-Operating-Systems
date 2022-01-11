#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <inttypes.h>
#include "client.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

int NUMFILES = 0;
int group = 0;
int clients = 0;


//message for the queue
struct msgbuf {
   long mtype;
   char mtext[256];
};

void createlist(char *path, char* buffer) {
    printf("File traversal and partitioning start\n");
    // if the given path is NULL, set it to the current directory
    if(path == NULL) {path = ".";}
    DIR *dir;
    struct dirent *dirent_ptr = NULL;
    
    // if the path cannot be opened, return error
    if((dir = opendir(path)) == NULL){
        printf("The path cannot be opened\n");
        exit(errno);
    }
 
    while((dirent_ptr = readdir(dir)) != NULL){
        // omit . and ..
        if(strncmp(dirent_ptr->d_name, ".", 1) != 0 || strncmp(dirent_ptr->d_name, "..", 1) != 0){
        // if the current reading entry is a directory
            if(dirent_ptr->d_type == DT_DIR){
                char new_path[100];
                strcpy(new_path, path);
                strcat(new_path, "/");
                strcat(new_path, dirent_ptr->d_name);
                createlist(new_path, buffer);
            }
            if (dirent_ptr->d_type != DT_DIR) {
                NUMFILES++; 

                strcpy(buffer, path);
                strcat(buffer, "/");
                
                strcat(buffer, dirent_ptr->d_name);

                node* newnode = create_node(buffer);

                node* array_addr;
                array_addr = &arr->array[group];
                insert(&array_addr, newnode);
                group = (group + 1) % clients;
            }
        }
    }
    closedir(dir);
}

void insert(node **phead, node *newnode) {
    //insert to the end of the list
    node* curr = *phead;

    if (curr->content == NULL){
		curr->content = newnode->content;
		return;
	}

    while (curr->next != NULL) {
        curr = curr->next;
    }

    curr->next = newnode;
}

//create new node
node* create_node(char *path)
{
	node* mynode = (node*)malloc(sizeof(node));
    mynode->content = (char*)malloc(256);
	strcpy(mynode->content, path);
    mynode->next = NULL;
	return mynode;
}

//release memory
void freenode(node* mynode){
    if(mynode != NULL){
        if(mynode->next != NULL){
            freenode(mynode->next);
        }
        free(mynode->content);
        free(mynode);
    }
}

void fileprintlist(node* phead, FILE* output) {
    node* cur = phead;
    while(cur != NULL) {
        fprintf(output,"%s\n",cur->content);
        cur = cur->next;
    }
}

int main(int argc, char** argv){
    printf("Client start\n");
	char* path = NULL;
    path = argv[1];
    if (argc != 3) {
		printf("Incorrect arg count. Enter file name and thread count\n");
		exit(EXIT_FAILURE);
	}

    clients = atoi(argv[2]);

    //allocate memory to the array for the lists
    arr =  (nodearray*) malloc(sizeof(nodearray)); 
    arr->array =  (node*) malloc(clients * sizeof(node)); 
    for (int i = 0; i < clients; i++) {
        arr->array[i].next = NULL;
    }

    char* buffer = (char*)malloc(256);
    createlist(argv[1], buffer);
    free(buffer);

    //create directory for the input
    int status;
    status = mkdir("ClientInput", 0777);

    if (status != 0 && errno != EEXIST) {
        printf("Cannot create input directory\n");
        exit(1);
    }
    
    //create directory for the output
    int status2;
    status2 = mkdir("Output", 0777);

    if (status2 != 0 && errno != EEXIST) {
        printf("Cannot create output directory\n");
        exit(1);
    }

    FILE *output;
	
    //create the partitioned list files
    for (int i = 0; i < clients; i++) {
        char buf[35];
        sprintf(buf, "ClientInput/Client%d.txt", i);

        output = fopen(buf, "w");
	
        if(!output) {
            printf("ERR Cannot open %s, exiting...\n",buf);
            exit(1);
        }

        fileprintlist(&arr->array[i], output);
        fclose(output);
    }

    struct msgbuf mybuf;
    struct msgbuf recbuf;

    key_t key = 100;
    int mid;
    // TODO: get message queue id
    mid = msgget(key, 0666 | IPC_CREAT);

    pid_t pid;
    for (int i = 0; i < clients; i++) {

        pid = fork();
        if (pid == 0) {
            //in the child process            
            mybuf.mtype = i+1;
            char previousMess[256];
            
            node* list = &arr->array[i];
            node* cur = list;

            //send each file path to the server
            while(cur != NULL) {
                memset(mybuf.mtext,0,strlen(mybuf.mtext));
                strcpy(mybuf.mtext, cur->content);
                //Keep track of previously sent message to ensure we don't catch it here.
                memset(previousMess,0,strlen(previousMess));
                strcpy(previousMess, cur->content);

                msgsnd(mid, (void*)&mybuf, sizeof(mybuf), 0);
                printf("Process %d sent path %s to Server\n", i + 1, mybuf.mtext);
                memset(mybuf.mtext,0,strlen(mybuf.mtext));   
                msgrcv(mid, (void*)&mybuf, sizeof(mybuf), i+1,0);

                while(strcmp(mybuf.mtext, previousMess) == 0){
                    msgsnd(mid, (void*)&mybuf, sizeof(mybuf), 0);

                    //i (client num) for the final message id key thing
                    memset(mybuf.mtext,0,strlen(mybuf.mtext));
                    msgrcv(mid, (void*)&mybuf, sizeof(mybuf), i+1,0);
                }  
                if(strcmp(mybuf.mtext,"ACK") == 0) {
                    printf("ACK has been sent to process %d for path %s\n", i+1, previousMess);
                }
                cur = cur->next;
            }
            memset(mybuf.mtext,0,strlen(mybuf.mtext));
            strcpy(mybuf.mtext, "END");
            msgsnd(mid, (void*)&mybuf, sizeof(mybuf), 0);
            printf("END has been sent by process %d\n", i+1);

            //i (client num) for the final message id key thing
            memset(mybuf.mtext,0,strlen(mybuf.mtext));
            msgrcv(mid, (void*)&mybuf, sizeof(mybuf), i+1,0);
            while(strcmp(mybuf.mtext, "END") == 0){
            
                memset(mybuf.mtext,0,strlen(mybuf.mtext));
                strcpy(mybuf.mtext, "END");
                msgsnd(mid, (void*)&mybuf, sizeof(mybuf), 0);

                //i (client num) for the final message id key thing
                memset(mybuf.mtext,0,strlen(mybuf.mtext));
                msgrcv(mid, (void*)&mybuf, sizeof(mybuf), i+1,0);
            }

            printf("Client process %d has received the final message from the server.\n", i +1 ); 
            
            FILE *output2;
            
            char buf2[35];
            sprintf(buf2, "Output/Client%d.txt", i);

            output2 = fopen(buf2, "w");

            if(!output2) {
                printf("cannot open: %s\n",buf2);
                printf("Exiting...\n");
                exit(1);
            }

            fprintf(output2, "%s\n", mybuf.mtext);

            fclose(output2);
            exit(0);
        }
    }
    
    //wait for child processes
    pid_t pidw;
    int statusw = 0;
    pidw = wait(&statusw);
    
    if(pidw > 0) {
    	pid = wait(&status);
    }

    for (int i = 0; i< clients; i++) {
        //freenode(&arr->array[i]);
    }

    // destory message queue
    msgctl(mid, IPC_RMID, NULL);

    return 0;
}