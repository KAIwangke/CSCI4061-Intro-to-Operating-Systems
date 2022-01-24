#include <stdio.h>     /* standard I/O functions                         */
#include <unistd.h>    /* standard unix functions, like getpid()         */
#include <signal.h>    /* signal name macros, and the signal() prototype */

/* first, define the Ctrl-C counter, initialize it with zero. */
int ctrl_c_count = 0;

void foo() {
	static int count = 0;
	printf("%d: Count = %d\n", time(NULL), ++count);
	sleep(1);
}
/* the Ctrl-C signal handler */
void catch_int(int sig_num)
{

			ctrl_c_count++;

}


int main(int argc, char* argv[])
{

		struct sigaction mask_set;	/* used to set a signal masking set. */
	  struct sigaction old_set;	/* used to store the old mask set.   */

		mask_set.sa_handler = catch_int;
		sigemptyset(&mask_set.sa_mask);
		mask_set.sa_flags = 0;

		sigaction(SIGINT,NULL,&old_set);
		if (old_set.sa_handler!=SIG_IGN) {
			sigaction(SIGINT,&mask_set,NULL);

		}

    /* enter an infinite loop of waiting for signals */
		while(1)
    /* restore the old signal mask */
{
			if (ctrl_c_count%2==0) {
				foo();
			}


}

    return 0;
}
