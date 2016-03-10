#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main () {

	int filho_pid;
	char *argv[] = { "echo", "Hello World", NULL };

	filho_pid = fork();
	
	if (filho_pid != 0 ) { 

}
	else {
		execv("/bin/echo",argv);
}
	return 0;
}
