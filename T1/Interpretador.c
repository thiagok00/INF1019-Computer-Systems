#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEFAULT_RR "execRR.txt"
#define DEFAULT_LOT "execLottery.txt"
#define DEFAULT_PRIO "execPrioridade.txt"





int main (void) {

	int resp,argc = 0;
	char argv[200];
	FILE *input = NULL;

	printf("Escolha o metodo de escalonamento:\n1. Round Robin\n2. Lottery\n3. Prioridade\n");
	scanf("%d",&resp);

	if(resp == 1)
		input = fopen(DEFAULT_RR,"r");
	else if (resp == 2)
		input = fopen(DEFAULT_LOT,"r");
	else if (resp == 3)
		input = fopen(DEFAULT_PRIO,"r");
	else {
		printf("Input errada.\n");
		exit(1);
	}
	if (input == NULL) {
		fprintf(stderr, "Erro abrir arquivo de input execs\n");
		exit(1);
	}


	if (resp == 1) {
		char str[20];
		int numtickets;
		while (fscanf(input,"Exec %s[^\n]",str,&pr) != EOF) {
			printf("%s %d\n",str,numtickets);
			argv[argc++] = str;
			argv[argc++] = numtickets;
		}
		execve("EscalonadorRR",argv);
	}
	if (resp == 2) {
		char str[20];
		int numtickets;
		while (fscanf(input,"Exec %s numtickets=%d [^\n]",str,&pr) != EOF) {
			printf("%s %d\n",str,numtickets);
			argv[argc++] = str;
			argv[argc++] = numtickets;
		}
		execve("EscalonadorLottery",argv);
	}

	if (resp == 3) {
		char str[20];
		int pr;
		while (fscanf(input,"Exec %s prioridade=%d [^\n]",str,&pr) != EOF) {
			printf("%s %d\n",str,pr);
			argv[argc++] = str;
			argv[argc++] = pr;
		}
		execve("EscalonadorPrioridade",argv);
	}







	return 0;
}