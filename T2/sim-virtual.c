#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void t_error(char* msg);


static FILE* output;

int main(int argc, char* argv[]) {

	unsigned addr;
	char rw;
	FILE *input;
	char *algoritmo,*pathInput;
	int tamPagina = 0, tamMem = 0, pageFault = 0, pageWrite = 0;
	int DEBUG = 0;


	/* Leitura de Parametros */
	if (argc < 5 || argc > 6) {
		t_error("Argumentos Insuficientes.");
	}

	algoritmo = argv[1];
	if ( strcmp(algoritmo,"LRU") != 0 && strcmp(algoritmo,"NRU") != 0 && strcmp(algoritmo,"SEG") != 0) {
		t_error("Algoritmo de substituicao invalido (LRU/NRU/SEG).");
	}

	pathInput = argv[2];
	input = fopen(pathInput,"r");
	if(input == NULL)
		t_error("Erro ao abrir arquivo entrada.");

	//TODO: duvida faixa valores 8-32 ou 8 16 32?
	sscanf(argv[3],"%d",&tamPagina);
	if(tamPagina < 8 || tamPagina > 32)
		t_error("Tamanho de pagina nao suportado. Apenas 8-32 KB");

	//TODO: Checar se entrada deve ser em KB e se 16384KB de fato são 16 MB
	sscanf(argv[4],"%d",&tamMem);
	if(tamMem < 126 || tamMem > 16384)
		t_error("Tamanho de memoria fisica invalido. Apenas 128-16384KB (16MB)");

	//TODO: Niveis de DEBUG
	if (argc == 6) {
		sscanf(argv[5],"-%d",&DEBUG);
		printf("Modo Debug Ativo Nivel Detalhamento %d.\n", DEBUG );
	}

	output = fopen("saida.txt","w");
	if (output == NULL)
		t_error("Erro abrir saida.txt");

	/**************************/
/*
Executando o simulador...
Arquivo de entrada: arquivo.log
Tamanho da memoria fisica: 128 KB
Tamanho das páginas: 8 KB
Alg de substituição: lru
Numero de Faltas de Páginas: 520
Numero de Paginas escritas: 352
*/
	printf("Executando o Simulador...\n");


	printf("Arquivo de Entrada: %s\n",pathInput );
	printf("Tamanho da Memoria fisica: %d KB\n",tamMem );
	printf("Tamanho das Paginas: %d KB\n",tamPagina );
	printf("Algoritmo de Substituicao: %s\n", algoritmo);
	printf("Numero de Faltas de Paginas: %d\n",pageFault);
	printf("Numero de Paginas Escritas: %d\n",pageWrite);




	while (fscanf(input, "%x %c ", &addr, &rw) != EOF)
		printf("%x %c\n",addr,rw );



	fclose(input);
	return 0;
}


void t_error(char* msg) {
	fprintf(stderr, "%s\n", msg);
	exit(1);
}