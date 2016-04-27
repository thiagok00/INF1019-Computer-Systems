#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#define FATIA_TEMPO 0.5
#define MAX 20

typedef struct proc Processos;
typedef struct ticket Ticket;
typedef struct processo Processo;


struct proc {
	Ticket *vet[MAX];
	int n;
};

struct ticket {
	int num;
	Processo *p;
};
struct processo {
	char nome[100];
	pid_t pid;
	pid_t pid_filho;
};

static Processos vtickets;
int *parouExecutar;

void criaProcesso(int n, char* procs[],int i) ;


int main (int argc, char* argv[]) {

	int i=0,tCriados = 1,segmento;
	FILE *saida;

	vtickets.n = 0;

	saida = fopen("SaidaLottery.txt","w");
	if (saida == NULL) {
		fprintf(stderr, "Erro abrir arquivo de saida\n");
		exit(1);
	}
	printf("uhul %d\n",argc);
	for (i = 0;i < argc; i++) {
		printf("argv %d: %s.\n",i,argv[i]);
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
	
	criaProcesso(argc, argv, tCriados);


	for (i = 0; i < vtickets.n ; i++){
		printf("-> %d %s %d %d\n",vtickets.vet[i]->num,vtickets.vet[i]->p->nome, vtickets.vet[i]->p->pid, vtickets.vet[i]->p->pid_filho );
	}




	shmdt (parouExecutar);
	shmctl (segmento, IPC_RMID, 0);
	return 0;
}

void criaProcesso(int n, char* procs[],int i) {

	int numTickets;
	Processo *p;
	
	if (i >= n)
		return;

	for (i = 1; i < n; i=i+2) {
		p = (Processo*) malloc(sizeof(Processo));
		if (p == NULL){
			fprintf(stderr, "Erro alocação memoria\n");
			exit(1);

		}
		strcpy(p->nome,procs[i]);
		sscanf(procs[i+1],"%d",&numTickets);
		printf("Criando processo: %s com %d tickets\n", p->nome,numTickets);

		while (numTickets) {
			Ticket *t;

			t = (Ticket*) malloc(sizeof(Ticket));
			if (t == NULL){
				fprintf(stderr, "Erro alocação memoria\n");
				exit(1);
			}

			t->num = 10;
			t->p = p;

			vtickets.vet[vtickets.n] = t;
			vtickets.n++;
			numTickets--;
		}

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
	}
}