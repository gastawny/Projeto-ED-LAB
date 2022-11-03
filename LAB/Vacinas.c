#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Pessoa{
char nome[15];
int idade, dose1, dose2; //considere valor 1 para vacinado e 0 não vacinado
char vacina[10]; //nome da vacina que tomou na 1ª dose
struct Pessoa *prox;
}TPessoa;

TPessoa *aloca(char nome[],int idade, int dose1, int dose2, char vacina[]);
TPessoa* unificar(TPessoa *postoY, TPessoa *postoZ);
TPessoa *pesquisa(TPessoa *lista, char nome[]);
void inserePessoa(TPessoa **p,char *nome, int idade, int dose1, int dose2, char *vacina );
void atualizaValor(TPessoa *lista, char NomeAntigo[], int novoValor);
void carregar(FILE *arq,TPessoa **pp);
void imprimeLista(TPessoa *p);
void desaloca(TPessoa **p);

int main()
{
	int op=1,op2 = 1;
	FILE *arquivo=NULL;
	FILE *arquivo2=NULL;
    TPessoa *postoY=NULL; 
	TPessoa *postoZ=NULL; 
	TPessoa *postoFinal1=NULL; 
   // TPessoa *prim=NULL;
	while(op!=5)
	{
		printf("\n1 Unificar arquivos de dados - Pessoas");
		printf("\n2 Busca pessoa - Vacinar");
		printf("\n3 Imprimir informacoes das pessoas");
		printf("\n4 Desalocar todas as pessoas");
		printf("\n5 Sair\n");
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
				    carregar(arquivo,&postoY);
					//printf("\nDados unificados com sucesso.\n");
					fclose(arquivo);
						}
					}

			if(arquivo2)
				printf("\nOs dados ja foram unificados.\n");
				else{
				arquivo2 = fopen("postoZ.dat", "r");
					if(!arquivo2)
					printf("Erro em abrir arquivo(s).\n");
					else{
				    carregar(arquivo2,&postoZ);
					printf("\nDados unificados com sucesso.\n");
					fclose(arquivo2);
						}
					}
			     		postoFinal1 = unificar( postoY, postoZ);
			}break;
			case 2: 
			{
                //imprimeLista(postoY);
			    //imprimeLista(postoZ);
				//imprimeLista(postoFinal1);
			}break;
			case 3: 
			{
						printf("\nInforme a opcao desejada: ");
						printf("\n1 - Pessoas Posto Y");
						printf("\n2 - Pessoas Posto Z");
						printf("\n3 - Pessoas Posto X e Y ");
						scanf("%d",&op2);
				switch(op2)
				{
						case 1: 
					{
						imprimeLista(postoY);
					}break;

						case 2: 
					{
						imprimeLista(postoZ);
					}break;

						case 3: 
					{
						imprimeLista(postoFinal1);
					}break;
				}
			}break;
			case 4: 
			{
                while(postoFinal1!=NULL)
	            {
                desaloca(&postoFinal1);
			    desaloca(&postoZ);
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
TPessoa* unificar(TPessoa *postoY, TPessoa *postoZ)
{
	int flag =0;
	TPessoa *postoFinal=NULL;
	TPessoa *postoInicio= postoY;
	if(postoY) //p!=NULL
	{
		while(postoY!=NULL)
		{
			inserePessoa(&postoFinal, postoY->nome, postoY->idade, postoY->dose1, postoY->dose2, postoY->vacina);
			postoY = postoY->prox;	
		}
	}
	postoY=postoInicio;
	if(postoZ) //p!=NULL
	{
		while(postoZ!=NULL)
		{	printf("\n Z Verificar -> %s\n",postoZ->nome);
			while(postoY!=NULL)
			{
					printf("\n Y -> %s",postoY->nome);
					printf("\n Flag -> %d",flag);
				// if((postoY->idade) == (postoZ->idade))
				if(strcmp(postoY->nome,postoZ->nome)== 0)
				{
					if(postoY->dose2 || postoZ->dose2 == 1)
					atualizaValor(postoFinal, postoY->nome, 1);

					flag++;
				}
				postoY = postoY->prox;	
			}
			printf("\n");
			postoY=postoInicio;
			if(flag==0)
			{
			inserePessoa(&postoFinal, postoZ->nome, postoZ->idade, postoZ->dose1, postoZ->dose2, postoZ->vacina);
			}
			postoZ = postoZ->prox;	
			flag=0;
		}
	}
	//imprimeLista(postoFinal);
	return postoFinal;
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
void carregar(FILE *arq,TPessoa **pp)
{
   char nome[15];
   char vacina[10];
   int idade,dose1,dose2;
   TPessoa *prim=NULL;
rewind(arq);

do{
fscanf(arq,"%s %d %d %d %s", nome,&idade,&dose1,&dose2,vacina);
//printf("\n %s %d %d %d %s\n",nome,idade,dose1,dose2,vacina);
inserePessoa(pp, nome, idade, dose1, dose2, vacina);
}while(!feof(arq));

//return PessoasY;
}

void inserePessoa(TPessoa **p,char *nome, int idade, int dose1, int dose2, char *vacina )//char nome[]
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
		printf("\n");
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

/// pesquisa se tem o nome repetido na lista
TPessoa *pesquisa(TPessoa *lista, char nome[]){
	TPessoa *p;
	for(p = lista; p!=NULL; p = p->prox)
		if(strcmp(p->nome,nome)== 0)
			return p;
	return NULL;
}
//atualiza a segunda dose pra 1 caso tenha tomado em pelo menos um dos postos
void atualizaValor(TPessoa *lista, char NomeAntigo[], int novoValor){
        TPessoa *aux = pesquisa(lista, NomeAntigo);
        if(!aux) return; // nao achou

	aux->dose2 = novoValor;
}

// maria 79 1 0 coronavac
// jose 81 1 1 coronavac
// renato 83 1 0 pfizer
// marilda 73 1 0 coronavac
// luiz 78 1 1 pfizer
// ecleia 71 1 0 coronavac
