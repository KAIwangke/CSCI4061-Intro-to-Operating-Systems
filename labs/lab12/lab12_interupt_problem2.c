#include <stdio.h>
#include <signal.h>

void foo() {

	static int count = 0;
	printf("%d: Count = %d\n", time(NULL), ++count);
	sleep(1);
}

int main() {
	//one program to toggle printing when sigint is received
	//another will not stop
	// TODO: use sigprocmask to block; or use sigaction to toggle.

	signal(SIGINT,foo);

	/* Print infinitely. */
	while (1) {foo();}
	return 0;


}
