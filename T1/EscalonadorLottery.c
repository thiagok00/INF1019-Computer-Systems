#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include "EscalonadorLottery.h"
#define FATIA_TEMPO 0.5
#define MAX 20

typedef struct proc Processos;

struct proc {
	int vet[MAX];
	int n;
};

struct ticket {
	int num;

};


static Processos p;

int main (int argc, char* argv[]) {

	int i,segmento, *parouExecutar;
	FILE *saida;
	int i;

	p.n = 0;

	saida = fopen("SaidaLottery.txt","w");
	if (saida == NULL) {
		fprintf(stderr, "Erro abrir arquivo de saida\n");
		exit(1);
	}

	for (i = 0;i < argc; i ++) {
		printf("%s",argv[i]);
	}





	return 0;
}

void criaProcesso() {


}