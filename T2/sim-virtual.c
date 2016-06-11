/****************************************************************************************************
 *
 *	TRABALHO 2 - Sistemas de Computação
 *	Professor: Markus Endler
 *	03/06/2016
 *
 *	Aluno:	João Pedro Garcia 1211768
 *	Aluno:	Thiago Klein      1321929
 *
 *
 *
 * O simulador deverá ter os seguintes quatro argumentos de linha de commando:	  	  
 *  a) o algoritmo de  susbtituição de página sendo simulado (LRU/NRU/SEG)	  
 *
 *  b) o arquivo contendo a	sequência de endereços de memoria acessados	(arq.log)	
 *  
 *  c) o tamanho de cada página/quadro de página em KB (tamanhos a serem suportados 8 a 32 KB)  
 *  d) o tamanho total de memoria física disponível em KB (faixa de valores: 128 KB a 16 MB).	
 *
 *  e) (opcional) Nivel detalhamento para modo DEBUG: -X 
 *
 * Exemplos:
 *		sim-virtual LRU simulador.log 16 128 
 *		sim-virtual NRU matriz.log 32 256 -1
 *
 * 
 * Cada	linha do arquivo (.log) conterá um endereço de memória acessado	(em representação hexa-­‐decimal),
 * seguido das letras R ou W, indicando um	acesso de leitura ou escrita, respectivamente.
 *
 * Exemplo:
 * 		0700ff10 R
 * 		2f6965a0 W
 *
 ****************************************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#define true 1
#define false 0


/*
 * Tipos
*/
typedef char bool;
typedef struct page Page;
typedef struct no No;
typedef struct lista Lista;

struct page {
	bool r; //FLAG pagina referenciada
	bool m; //FLAG pagina modificada
	bool present;
	int ultimoAcesso;
};

struct no {
	No *ant, *prox;
	Page *elem;
};

struct lista {
	int n;
	No *ini,*fim;
	
};


/*
 * Funcoes
*/

Lista* lista_cria();
void  lista_insere(Lista *f, Page *p);
Page* lista_removefinal(Lista *f);
int lista_removeelem(Lista *f, Page *p);


void t_error(char* msg);
void t_lru(Page *page, Lista *memFisica, int *pageWrite);
int t_nru(Page *memVirtual, int *memFisica, int qtdQuadros, int indexPagina, int *pageWrite);
int t_seg(Page *memVirtual, int *memFisica, int qtdQuadros, int indexPagina, int *pageWrite);


/*
 * Variaveis Globais
*/
static FILE* output;

int main(int argc, char* argv[]) {

	unsigned addr;
	char rw;
	FILE *input;
	char *algoritmo,*pathInput;
	int i,tamPagina = 0, tamMem = 0, pageFault = 0, pageWrite = 0, qtdQuadros, aux, bitsIndice, maximoPaginas, timer;
	int DEBUG = 0;
	Page *memVirtual;
	Lista* memFisica;

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

	printf("Executando o Simulador...\n");

	/* Quantidade de quadros na memoria fisica */
	qtdQuadros = tamMem/tamPagina;

	/* Pegando a quantidade de bits relativos ao indice da pagina */
	aux = tamPagina*1024;
	bitsIndice = 0;
	while(aux > 1) {
		bitsIndice++;
		aux /= 2;
	}

	/* Pegando tamanho maximo da tabela de paginas */
	aux = 32 - bitsIndice;
	maximoPaginas = 2;
	while (aux) {
		maximoPaginas *= 2;
		aux--;
	}

	memVirtual = (Page*) malloc(maximoPaginas*sizeof(Page));
	if(memVirtual == NULL)
		t_error("Memoria Insuficiente para memoria virtual.");
	printf("MAXIMO PAGINA %d\n",qtdQuadros );
	for(i=0; i < maximoPaginas; i++) {
		Page *p = &memVirtual[i];
		p->r = false;
		p->m = false;
		p->ultimoAcesso = -1;
		p->present = false;
	}

	memFisica = lista_cria();

	timer = 0;
	while (fscanf(input, "%x %c ", &addr, &rw) != EOF) {
		
		unsigned pageIndex;
		Page *p;
		
		pageIndex = addr >> bitsIndice;
	 	p = &memVirtual[pageIndex];

		if(p->present) {
			lista_removeelem(memFisica, p);
			lista_insere(memFisica, p);
		}
		else {
			pageFault++;
			//Ainda existem quadros disponiveis na memoria
			if(memFisica->n < qtdQuadros) {
				lista_insere(memFisica,p);
			}
			else {	//Sem espaço, chama algoritmo e substituicao
			//	t_lru(p,memFisica,&pageWrite);
			}
		}
		p->r = true;
		p->present = true;

		if (rw == 'W')
			p->m = true;
		p->ultimoAcesso = timer;

		timer++;		
	}
	

	printf("Arquivo de Entrada: %s\n",pathInput );
	printf("Tamanho da Memoria fisica: %d KB\n",tamMem );
	printf("Tamanho das Paginas: %d KB\n",tamPagina );
	printf("Algoritmo de Substituicao: %s\n", algoritmo);
	printf("Numero de Faltas de Paginas: %d\n",pageFault);
	printf("Numero de Paginas Escritas: %d\n",pageWrite);

	free(memVirtual);
	fclose(input);
	fclose(output);

	return 0;	
}

void t_error(char* msg) {
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

Lista* lista_cria() {
	Lista *l;

	l = (Lista *) malloc(sizeof(Lista));
	if (l == NULL)
		t_error("Memoria insuficiente");
	l->n = 0;
	l->ini = NULL;
	l->fim = NULL;
	return l;
}

void  lista_insere(Lista *f, Page *p) {

	No* nv;

	nv = (No*) malloc(sizeof(No));
	if (nv == NULL)
		t_error("Memoria Insuficiente");

	nv->elem = p;
	nv->prox = f->ini;
	nv->ant = NULL;
	if(f->ini != NULL)
		f->ini->ant = nv;
	f->ini = nv;
	f->n++;
	return;
}

Page* lista_removefinal(Lista *f) {

	No* aux;
	Page *ret;

	if(f->n == 0)
		return NULL;

	ret = f->fim->elem;
	aux = f->fim;
	f->fim = aux->ant;
	
	if(f->fim != NULL)
		f->fim->prox = NULL;
	
	free(aux);
	f->n--;
	return ret;
}

int lista_removeelem(Lista *f, Page *p){

	No* aux = f->ini;
	while(aux != NULL && aux->elem != p);
	if (aux == NULL)
		return -1;

	if(aux->ant == NULL && aux->prox == NULL) {
		free(aux);
		f->ini = NULL;
		f->fim = NULL;
		f->n--;

	}
	else if (aux->ant == NULL) {
		No* excl = aux;
		f->ini = aux->prox;
		free(excl);
		f->ini->ant = NULL;
		f->n--;
	}
	else if (aux->prox == NULL) {
		lista_removefinal(f);		
	}
	else {
		f->n--;
		aux->ant->prox = aux->prox;
		aux->prox->ant = aux->ant;
		free(aux);
	}
	return 1;
}


void t_lru(Page *page, Lista *memFisica, int *pageWrite) {
	Page *p = lista_removefinal(memFisica);
	

	lista_insere(memFisica,page);
	p->r = false;
	if (p->m == true)
		(*pageWrite)++;
	p->m = false;
	p->present = false;


}

int t_nru(Page *memVirtual, int *memFisica, int qtdQuadros, int indexPagina, int *pageWrite) {return 0;}
int t_seg(Page *memVirtual, int *memFisica, int qtdQuadros, int indexPagina, int *pageWrite) {return 0;}
