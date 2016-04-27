#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_RR "execRR.txt"
#define DEFAULT_LOT "execLottery.txt"
#define DEFAULT_PRIO "execPrioridade.txt"





int main (void) {

	int resp,i = 0;
	char *argv[200];
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
		while (fscanf(input,"Exec %s[^\n]",str) != EOF) {
			printf("%s\n",str);
			argv[i++] = str;
		}
		execv("EscalonadorRR",argv);
	}
	if (resp == 2) {
		char aux[20];
		int numtickets;
		argv[i++] = "EscalonadorLottery";
		while (fscanf(input,"Exec %s numtickets=%d [^\n]",aux,&numtickets) != EOF) {
			char *str3 = (char*) malloc(sizeof(char)*20);
			char *str4 = (char*) malloc(sizeof(char)*4);
			strcpy(str3,aux);
			argv[i++] = str3;
			sprintf(str4,"%d",numtickets);
			argv[i++] = str4;
		}
		argv[i] = 0;
		execv("EscalonadorLottery",argv);
	}

	if (resp == 3) {
		char str[20];
		char str2[3];
		int pr;
		while (fscanf(input,"Exec %s prioridade=%d [^\n]",str,&pr) != EOF) {
			printf("%s %d\n",str,pr);
			argv[i++] = str;
			sprintf(str2,"%d",pr);
			argv[i++] = str2;
		}
		execv("EscalonadorPrioridade",argv);
	}

	return 0;
}