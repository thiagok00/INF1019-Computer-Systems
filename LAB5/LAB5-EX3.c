#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main (void) {

	int fd,fd2,aux,pid;


	if ((fd=open("entrada.txt", O_RDONLY,0666)) == -1)  {
		return -1;
	}
	if ((fd2=open("saida.txt", O_WRONLY|O_CREAT|O_TRUNC,0666)) == -1)  {
		return -1;w
	}



	if (dup2(fd,0) == -1) {
		return -2;	
	}
	if (dup2(fd2,1) == -1) {
		return -3;	
	}
	

	while (scanf("%d",&aux) != -1) {
		printf("Mult: %d\n",aux*10);
	}
	
	return 0;
}
