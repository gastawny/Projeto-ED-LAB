#include<stdio.h>
#include<stdlib.h>
#include<string.h>


typedef struct Pessoa{
char nome[15];
int idade, dose1, dose2; //considere valor 1 para vacinado e 0 não vacinado
char vacina[10]; //nome da vacina que tomou na 1ª dose
struct Pessoa *prox;
}TPessoa;

TPessoa* unificar(TPessoa *p1, TPessoa *p2); 
void inserePessoaY(TPessoa **p,char *nome, int idade, int dose1, int dose2, char *vacina );
TPessoa *aloca(char nome[],int idade, int dose1, int dose2, char vacina[]);
void carregarY(FILE *arq,TPessoa **pp);
void imprimeLista(TPessoa *p);
void desaloca(TPessoa **p);
int main()
{
	int op=1,linhas, FLAG=0;
	FILE *arquivo=NULL;
    TPessoa *postoY=NULL; 
   // TPessoa *prim=NULL;
	while(op!=5)
	{
		printf("\n1 Unificar arquivos de dados - Pessoas");
		printf("\n2 Busca pessoa - Vacinar");
		printf("\n3 Imprimir informacoes das pessoas");
		printf("\n4 Desalocar todas as pessoas");
		printf("\n5 Sair");
		scanf("%d",&op);
		switch(op)
		{
			case 1: 
			{
			if(arquivo)
				printf("\nOs dados ja foram unificados.\n");
				else{
				arquivo = fopen("postoY.dat", "r");
					if(!arquivo)
					printf("Erro em abrir arquivo(s).\n");
					else{
				    carregarY(arquivo,&postoY);
					printf("\nDados unificados com sucesso.\n");
					fclose(arquivo);
						}
					}
			}break;
			case 2: 
			{
                imprimeLista(postoY);
			}break;
			case 3: 
			{
			
			}break;
			case 4: 
			{
                while(postoY!=NULL)
	            {
                desaloca(&postoY);
                }
                printf("\nPessoas desalocadas\n");
			}break;

            case 5: 
			{
				printf("\nPrograma Finalizado\n");
			}break;

			default:
				printf("\nOpção inválida.\n");
		}
	}
}


TPessoa *aloca(char nome[],int idade, int dose1, int dose2, char vacina[])
{
	TPessoa *novo;
	novo = (TPessoa *)malloc(sizeof(TPessoa));
	if(novo == NULL) return NULL;
	strcpy(novo->nome, nome);
	novo->idade = idade;
	novo->dose1 = dose1;
    novo->dose2 = dose2;
    strcpy(novo->vacina, vacina);
	novo->prox = NULL;
	return novo;
}
//abre o arquivo
void carregarY(FILE *arq,TPessoa **pp)
{
   char nome[15];
   char vacina[10];
   int idade,dose1,dose2;
   TPessoa *prim=NULL;
rewind(arq);

do{
fscanf(arq,"%s %d %d %d %s", nome,&idade,&dose1,&dose2,vacina);
//printf("\n %s %d %d %d %s\n",nome,idade,dose1,dose2,vacina);
inserePessoaY(pp, nome, idade, dose1, dose1, vacina);
}while(!feof(arq));

//return PessoasY;
}

void inserePessoaY(TPessoa **p,char *nome, int idade, int dose1, int dose2, char *vacina )//char nome[]
{
	TPessoa *novo;
	novo = aloca(nome, idade, dose1, dose2, vacina);
	if(novo==NULL) return;
	novo->prox = *p;
	*p = novo;
}


void imprimeLista(TPessoa *p)
{
	//assert(p);
	if(p) //p!=NULL
	{
		while(p!=NULL)
		{
			printf("\n%s %d %d %d %s",p->nome, p->idade, p->dose1, p->dose2, p->vacina);
			p = p->prox;	
		}
	}
	else
		printf("\nLista vazia!");
}

void desaloca(TPessoa **p)
{
	TPessoa *aux;
	if(*p==NULL) return; //if(!p)
	aux = *p;//olha para o 1o
	*p = (*p)->prox;
	free(aux);
	aux=NULL;
}



// maria 79 1 0 coronavac
// jose 81 1 1 coronavac
// renato 83 1 0 pfizer
// marilda 73 1 0 coronavac
// luiz 78 1 1 pfizer
// ecleia 71 1 0 coronavac