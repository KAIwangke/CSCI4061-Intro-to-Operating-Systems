#ifndef CLIENT_H
#define CLIENT_H

#define PATHSIZE = 256;

typedef struct linked_node {
	char *content;
	struct linked_node *next;	
} node;

typedef struct node_array {
    node* array;
} nodearray;

void insert(node**, node*);
//void insert(node*, node*);
node* create_node(char *);
void lscount(char *path, bool recurse_flag);
void freenode(node* mynode);

//head of the list
node* head;
nodearray* arr;

#endif