#include "graph.h"

struct AdjListNode* newAdjListNode(int dest){ 
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode)); 
    newNode->dest = dest; 
    newNode->next = NULL; 
    return newNode; 
} 

struct DepGraph* createDepGraph(FILE *input, char cmds[][550]){
	ssize_t read;
	size_t len = 0;
	char *line = NULL;
    int V;

    // number of nodes
    read = getline(&line, &len, input);     
    sscanf(line, "%d", &V);

    // skip the blank line
    read = getline(&line, &len, input);

    int cmdIter = 0;
	while((read = getline(&line, &len, input)) != -1) {
		if(strcmp(line, "\n") == 0){
			break;
		}
		// read commands
		strcpy(cmds[cmdIter++], line);
	}

    // Graph creation
    struct DepGraph* graph =  (struct DepGraph*) malloc(sizeof(struct DepGraph)); 
    graph -> V = V;  
    graph -> array =  (struct AdjList*) malloc(V * sizeof(struct AdjList)); 

    int i; 
    for (i = 0; i < V; ++i){
        graph->array[i].head = NULL;
        graph->array[i].visit = 0;
        // TODO: you can add corresponding cmd to each node, it depends on you
        // if you want to save them to graph, feel free to change graph.h and anything needed
        // otherwise you don't need to do anything here but you will extract
        // corresponding command from cmds in DFSVisit
    }

    while((read = getline(&line, &len, input)) != -1) {
        // TODO: extract src and dest from current line
        // you may find strtok very helpful
        int source, dest;

        source = atoi(strtok(line, " "));
        dest = atoi(strtok(NULL, " "));

        // TODO: add current edge to graph (src <-> dest forms an edge)
        addEdge(graph, source, dest);
	}
	
	free(line);
	
    return graph; 
}


void addEdge(struct DepGraph* graph, int src, int dest){
    // TODO: add an edge to graph
    struct AdjListNode* newNode = newAdjListNode(dest);
    struct AdjListNode* curr = graph->array[src].head;
    if(curr == NULL){
        graph->array[src].head = newNode;
    }
    else{
        while(curr->next != NULL){
            curr = curr->next;
        }
        curr->next = newNode;
    }
}

void DFSVisit(struct DepGraph* graph, int node, char cmds[][550], int mode) {
    // start from node, iterate over its adjListNode
    struct AdjListNode* adjListNode = graph -> array[node].head;

    if(graph->V!=1){    //base case
        while(adjListNode != NULL){
            // TODO: fork then call DFSVisit inside child process recursively
            // TODO: inside parent process, wait child process or not depend on mode
            // TODO: go to next adjacent node
            pid_t child=fork();

            if (child == 0) {
                if (graph->array[node].visit == 0){
                    graph->array[node].visit = 1;
                    DFSVisit(graph, adjListNode->dest, cmds, mode);
                }
            }
            else if (mode == 0){
                wait(NULL);
            }
            //in the par need to wait till the last command line
            adjListNode = adjListNode->next;
        }
    }
    // MODE 1
    if(mode==1){
            //trap parent process
            pid_t pid;
            int status=0;
            
            pid = wait (&status);
            //wait returns pid of any child processes and returns 0 if none
            while (pid > 0) {
                pid = wait(&status);
            }
     }

    // TODO: wait child processes or not depend on mode

    // TODO: print current node command to results.txt

    FILE *output;
	output = fopen("results.txt", "a");
	
	if(!output) {
		printf("cannot open %s", "results.txt");
		printf("Exiting...\n");
		exit(1);
	}
	
    char *mycommand = cmds[node];
    fprintf(output,"%d %d %s",getpid(),getppid(),mycommand);
    fclose(output);
    // TODO: execute current node command
    // system(mycommand);
    execl("/bin/sh", "sh", "-c", mycommand, (char *) NULL);
    //Using system call

    exit(0);
}

void processGraph(struct DepGraph* graph, char cmds[][550], int mode){
    int i;
    int child = fork();
    if (child == 0) {
	   DFSVisit(graph, 0, cmds, mode);
    }
    else{
        wait(NULL);
    }
}

void freegraph(struct DepGraph* graph){
    freearray(graph);
    free(graph);
}

void freearray(struct DepGraph* graph){
    for (int i = 0; i < graph->V; ++i){
        freenode(graph->array[i].head);
    }
    free(graph->array);
}

void freenode(struct AdjListNode* mynode){
    if(mynode != NULL){
        if(mynode->next != NULL){
            freenode(mynode->next);
        }
        free(mynode);
    }
}