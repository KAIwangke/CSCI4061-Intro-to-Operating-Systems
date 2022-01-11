#ifndef SERVER_H
#define SERVER_H
#define size 256
#define MSGSZ 256
struct msgbuf {
   long mtype;
   char mtext[MSGSZ];
};

struct argstruct {
	int client;
};

void* handle_multiple(void* clients);
void parsing(char input[size]);

#endif