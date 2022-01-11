#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "linked_list.h"
#include <unistd.h>
#include <time.h>


//global line-number counter to be sync'ed.
int line_ctr = 0;

//initialization of mutex locks
pthread_mutex_t line_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t list_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t seq_lock = PTHREAD_MUTEX_INITIALIZER;
//at least two mutex locks should be used

struct argstruct {
	char* fname;
	int flines;
};

//start of the threads
void* process_file(void *param)
{
	struct argstruct* args = (struct argstruct*)param;

	while (line_ctr < args->flines) {
		pthread_t self;
		self = pthread_self();

		//lock access to line_ctr
		if (pthread_mutex_lock(&line_lock) != 0) {
			perror("error locking line\n");
			exit(1);
		}
		char* line;
		node* newnode = NULL;
		if (line_ctr < args->flines) {
			line = read_line(args->fname, line_ctr);
			newnode = create_node(line_ctr, line);
			line_ctr++;
		}
		if (pthread_mutex_unlock(&line_lock) != 0) {
			perror("error unlocking line\n");
			exit(1);
		}

		/*
		srand(time(NULL));
		int sleepytime = rand() % 5 + 1;
    	printf("Sleeping for %d seconds...\n", sleepytime);
    	sleep(sleepytime);
    	printf("Wakey wakey!\n");*/

		//lock access to the linked list
		if (pthread_mutex_lock(&list_lock) != 0) {
			perror("error locking list\n");
			exit(1);
		}
		
		if (newnode != NULL) {
			insert(&head, newnode);
		}
		if (pthread_mutex_unlock(&list_lock) != 0) {
			perror("error unlocking list\n");
			exit(1);
		}		
	}

}

int main(int argc, char* argv[])
{
	FILE* in_ptr;
	int threads;
	char *filename = NULL;

	if (argc != 3) {
		printf("Incorrect arg count. Enter file name and thread count\n");
		exit(EXIT_FAILURE);
	}

	threads = atoi(argv[2]);
	if (threads < 1 || threads > 16) {
		printf("Incorrect number of threads. Enter between 1 and 16\n");
		exit(EXIT_FAILURE);
	}

	//get the filename
	filename = argv[1];

	//count the number of lines in the file
	int c, last = '\n';
	int lines;
	FILE *in;
	in=fopen(filename,"r");
	if (in == NULL) {
		perror("file does not exist");
		exit(1);
	}
	while((c=getc(in))!=EOF) {
		//printf("in the while\n");
		if (c == '\n') {
			lines++;
		}
	}
	fclose(in);

	//add the filename and number of lines to the agument struct
	struct argstruct* arg = (struct argstruct*)malloc(sizeof(struct argstruct));
	arg->fname = filename;
	arg->flines = lines;

	//array of thread ids
	pthread_t m_threads[threads];

	//create the threads and have them operate process_file
	for(int i = 0; i < threads; i++) {
		if (pthread_create(&m_threads[i], NULL, process_file, (void*)arg) != 0) {
			perror("error creating thread\n");
			exit(1);
		}
    }

	//join the threads
	for (int i = 0; i < threads; i++) {
		if (pthread_join(m_threads[i], NULL) != 0) {
			perror("error joining thread\n");
			exit(1);
		}
	}

	//print out the finished linked list
	traversal(head);

	//free the list memory
	free(head);
	
	//free the argument struct memory
	free(arg);
	return 0;
}
