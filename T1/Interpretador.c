#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEFAULT_RR "execRR.txt"
#define DEFAULT_LOT "execLottery.txt"
#define DEFAULT_PRIO "execPrioridade.txt"





int main (void) {

	int resp;
	FILE *input;

	printf("Escolha o metodo de escalonamento:\n1. Round Robin\n2. Lottery\n 3. Prioridade\n");
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
		int pr;
		while (fscanf(in,"Exec %s prioridade=%d",str,&pr) != EOF) {
			printf("%s %d\n",str,pr);
		}
	}







	return 0;
}