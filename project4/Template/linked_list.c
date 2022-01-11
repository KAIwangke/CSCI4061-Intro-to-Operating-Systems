#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pthread.h"
#include "linked_list.h"

// sequence counter
int seq_ctr = 0;

//Read the file on a line by line basis
char* read_line(char* fname, int line_no) 
{
    FILE *fp = fopen(fname, "r");
    if(fp == NULL) printf("readline Cannot open the file.\n");
    
	char* buf = malloc(LINE_SIZE);

	for (int i = 0; i < line_no; i++) {
		if (fgets(buf, LINE_SIZE, fp) != NULL) {
			//spit out lines
		}
	}
	char* out;

	if (fgets(buf, LINE_SIZE, fp) != NULL) {
		out = buf;
		//return the line thats needed.
	}
    fclose(fp);

	return out;
} 

//traverse the linked list
void traversal(node *head)
{	
	//the first is the head, (not part of the list)
    node *temp = head;// create the node for traversal
    while(temp != NULL){
        printf("%d,%d,%s\n", temp->seq_no, temp->line_no, temp->content);
        temp = temp->next;
    }
	
}

void freenode(node* mynode){
    if(mynode != NULL){
        if(mynode->next != NULL){
            freenode(mynode->next);
        }
        free(mynode);
    }
}

// insert the node into the linked list
void insert(node **phead, node *newnode)
{
	newnode->seq_no = seq_ctr;
	seq_ctr++;
	
	node* current = *phead;
	node* previous = NULL;
	
	//The linked list is empty
	if (*phead == NULL){
		*phead = newnode;
		return;
	}
	//The new line number is before the current head's
	else if(current->line_no > newnode->line_no){
		newnode->next = current;
		*phead = newnode;
		return;
	}
	else{
		while(current != NULL){
			//newnode should occur before nextnode
			if(current->line_no > newnode->line_no){
				newnode->next = current;
				previous->next = newnode;
			}
			previous = current;
			current = current->next;
		}
		//newnode is the last known line
		if (current == NULL){
			previous->next = newnode;
		}
	}
	return;
}

//create a new node structure
node* create_node(int line_no, char *line)
{
	node* mynode = (node*)malloc(sizeof(node));
	mynode->line_no = line_no;
	mynode->content = line;
	mynode->next = NULL;
	mynode->seq_no = 0;

	return mynode;
}
