#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#define FATIA_TEMPO 0.5


typedef struct processo Processo;
typedef struct fila Fila ;

struct processo {
	char nome[100];
	pid_t pid;
	pid_t pid_filho;
	Processo *prox;
};

struct fila {
	Processo *ini;
	Processo *fim;
};


void queue(Fila *f,Processo *p);
Processo* dequeue(Fila *f);



int main (int argc, char* argv[]) {
	
	int i,segmento, *parouExecutar;
	Processo *p;
	Fila *f;
	FILE *saida;

	f = (Fila*) malloc(1*sizeof(Fila));
	if (f == NULL) {
		fprintf(stderr, "Erro ao alocar memoria\n");
		exit(1);
	}

	saida = fopen("SaidaRR.txt","w");
	if (saida == NULL) {
		fprintf(stderr, "Erro abrir arquivo de saida\n");
		exit(1);
	}

	segmento = shmget (IPC_PRIVATE, sizeof (int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	if (segmento == -1) {
		fprintf(stderr, "Erro criar area memoria compartilhada\n");
		exit(1);
	}
	parouExecutar = (int*) shmat(segmento, 0, 0);
	if (parouExecutar == NULL) {
		fprintf(stderr, "Erro ao anexar segmento de memoria compartilhada\n");
		exit(1);
	}

	//Criando todos os processos
	for(i = 1; i < argc; i++) {
		
		p = (Processo *) malloc(1*sizeof(Processo));
		if (p == NULL) {
			fprintf(stderr, "Erro alocacao memoria\n");
			exit(1);
		}

		sscanf(argv[i], "%s", p->nome); 
		p->prox = NULL;

		if ((p->pid = fork()) < 0) {
			fprintf(stderr, "Erro ao criar filho\n");
			exit(1);
		}

		if (p->pid == 0) { //Filho

			if ((p->pid_filho = fork()) < 0 ) {
				fprintf(stderr, "Erro ao criar filho\n");
				exit(1);
			}
			if(p->pid_filho == 0) {
				char *const args[] = {p->nome, 0};	
				execv(p->nome,args);
			}
			else {
				int status;
				waitpid(p->pid_filho, &status,0);
				*parouExecutar = 1;
				exit(1);
			}
		}
		else { //Pai
			kill(p->pid,SIGSTOP);
		}
		queue(f,p);
	}


	while ((p = dequeue(f)) != NULL) {

		*parouExecutar = 0;

		kill(p->pid, SIGCONT);
		kill(p->pid_filho, SIGCONT);
		sleep(FATIA_TEMPO);
		kill(p->pid, SIGSTOP);
		kill(p->pid_filho, SIGSTOP);

		if((*parouExecutar) == 1) {
			*parouExecutar = 0;
			free(p);
		}
		else {
			queue(f,p);
		}
	}

	shmdt (parouExecutar);
	shmctl (segmento, IPC_RMID, 0);
	fclose(saida);

	return 0;
}

void criaProceso()


void queue(Fila *f, Processo *p) {

	if (f->fim != NULL) 
		f->fim->prox = p;
	else
		f->ini = p;
	f->fim = p;
}

Processo* dequeue(Fila *f) {

	Processo *p;

	if (f->ini == NULL)
		return NULL;

	p = f->ini;
	f->ini = p->prox;
	if (f->ini == NULL){
		f->fim = NULL;
	}
	return p;
}