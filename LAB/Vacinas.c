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
TPessoa* unificar(TPessoa *postoY, TPessoa *postoZ,TPessoa *postoFinal);
TPessoa *pesquisa(TPessoa *lista, char nome[]);
int NomeVacina(char vacina[]);
char* strlwr1(char s[]);
void learquivo(char arquivo[]);
void inserePessoa(TPessoa **p,char *nome, int idade, int dose1, int dose2, char *vacina );
void atualizaValor(TPessoa *lista, char NomeAntigo[],int dose, int novoValor);
void carregar(FILE *arq,TPessoa **pp);
void imprimeLista(TPessoa *p);
void desaloca(TPessoa **p);
void verificaVacina(TPessoa **lista, char Nome[]);
void criaArquivo(TPessoa * p);

int main()
{
	int op=1,op2 = 0;
	char ConsultaNome[15];
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
			if(postoY)
				printf("\nOs dados ja foram unificados.\n");
				else{
				arquivo = fopen("postoY.dat", "r");
					if(!arquivo)
					printf("Erro em abrir arquivo postoY.\n");
					else{
				    carregar(arquivo,&postoY);
					//printf("\nDados unificados com sucesso.\n");
					fclose(arquivo);
						}
					}

			if(postoZ)
				printf("\nOs dados ja foram unificados.\n");
			else{
				arquivo2 = fopen("postoZ.dat", "r");
				if(!arquivo2)
					printf("Erro em abrir arquivo postoZ.\n");
				else{
					carregar(arquivo2,&postoZ);
					printf("\nDados unificados com sucesso.\n");
					fclose(arquivo2);
				}
			}
			postoFinal1 = unificar( postoY, postoZ,postoFinal1);
			}break;

			case 2: 
			{
                printf("\nInforme o nome de deseja consultar: ");
            	scanf("%s",ConsultaNome);
				verificaVacina(&postoFinal1, ConsultaNome);    
			}break;

			case 3: 
			{
                op2=0;
                while(op2!=1 && op2!=2 && op2!=3 )
	            {
						printf("\nInforme a opcao desejada: ");
						printf("\n1 - Pessoas Posto Y");
						printf("\n2 - Pessoas Posto Z");
						printf("\n3 - Pessoas Posto X e Y ");
						scanf("%d",&op2);
                    switch(op2)
                    {
                    	case 1: learquivo("postoY.dat");break;
						case 2: learquivo("postoZ.dat");break;
						case 3: imprimeLista(postoFinal1);break;
                        default: printf("\nOpcao invalida.\n");
                    }
                }
			}break;

			case 4: 
			{
                if(postoFinal1)
                {
                    while(postoFinal1!=NULL)
                    {
						desaloca(&postoFinal1);
						desaloca(&postoZ);
						desaloca(&postoY);
                    }
                    printf("\nPessoas desalocadas\n");
                }
                else 
                	printf("Nao existe pessoas para desalocar:)\n");
			}break;

            case 5: 
			{
                if(postoFinal1)
                {
                    criaArquivo(postoFinal1);
                    while(postoFinal1!=NULL)
                    {
						desaloca(&postoFinal1);
						desaloca(&postoZ);
						desaloca(&postoY);
                    }
                }

				printf("\nPrograma Finalizado\n");
			}break;

			default:
				printf("\nOpcao invalida.\n");
		}
	}
}
TPessoa* unificar(TPessoa *postoY, TPessoa *postoZ, TPessoa *postoFinal)
{
	int flag = 0;
	TPessoa *postoInicio= postoY;
	if(postoY)
	{
		while(postoY!=NULL)
		{
			inserePessoa(&postoFinal, postoY->nome, postoY->idade, postoY->dose1, postoY->dose2, postoY->vacina);
			postoY = postoY->prox;	
		}
	}
	postoY=postoInicio;
	if(postoZ)
	{
		while(postoZ!=NULL)
		{	
			while(postoY!=NULL)
			{
				
				if(strcmp(postoY->nome,postoZ->nome)== 0)
				{
					if(postoY->dose2 || postoZ->dose2 == 1)
					atualizaValor(postoFinal, postoY->nome,2, 1);

					flag++;
				}
				postoY = postoY->prox;	
			}
			postoY = postoInicio;
			if(flag==0)
				inserePessoa(&postoFinal, postoZ->nome, postoZ->idade, postoZ->dose1, postoZ->dose2, postoZ->vacina);
			postoZ = postoZ->prox;	
			flag=0;
		}
	}
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
		inserePessoa(pp, nome, idade, dose1, dose2, vacina);
	}while(!feof(arq));
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
    char d1[4] = "Nao";
    char d2[4] = "Nao";
	//assert(p);
	if(p) //p!=NULL1
	{
		while(p!=NULL)
		{
			if(p->dose1==1)
			strcpy(d1, "Sim");

			if(p->dose2==1)
			strcpy(d2, "Sim");

			printf("\n%10s  Idade:%3d  1-Dose:%3s   2-Dose:%3s  %s\n",p->nome, p->idade ,d1 ,d2, p->vacina);
			p = p->prox;	
		}
		printf("\n");
	}
	else
		printf("\nA Lista nao foi unificada!");
}

void desaloca(TPessoa **p)
{
	TPessoa *aux;
	if(*p==NULL) return; //if(!p)
	aux = *p;
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
void atualizaValor(TPessoa *lista, char Nome[],int dose, int novoValor){
        TPessoa *aux = pesquisa(lista, Nome);
        if(!aux) return; // nao achou
        if(dose == 1)
			aux->dose1 = novoValor;
        else if(dose == 2)
	    	aux->dose2 = novoValor;
}
void verificaVacina(TPessoa **lista, char Nome[]){
	char vacina[10];
	int idade,flag=0,vac=0;
        TPessoa *aux = pesquisa(*lista, Nome);
        if(!aux) 
		{
			printf("\n------Pessoa nao cadastrada------\n");
			printf("\nInforme os dados para o cadastro:\n");
			do
            {
				printf("\nIdade: ");
				scanf("%d",&idade);
            } while(idade<=0);
			printf("\n-----Informe o nome de uma vacina valida-----\n");
            printf("\nAstraZeneca -  CoronaVac - Fiucruz\n Janssen - Pfizer - Sputnik");
			do
			{
				printf("\nNome da vacina: ");
				scanf("%s",vacina);
			}while(NomeVacina(vacina)==0);

			printf("\n------Pessoa cadastrada com sucesso------\n");
			printf("\nDeve tomar a primeira Dose\n");
            do
            { 
                printf("\nConfirme se a vacina foi aplicada 1-Sim 2-Nao\n");
                scanf("%d",&vac);
                if(vac==1)
		        	inserePessoa(lista, Nome, idade, 1, 0, vacina);
                else
                	inserePessoa(lista, Nome, idade, 0, 0, vacina);
            }while(vac!=1 && vac!=2);
			return; // nao achou
		}
		if((aux->dose1) == 0)
			printf("\nDeve tomar a primeira Dose\n");
		else if((aux->dose2) == 0)
		{
			printf("\nDeve tomar a segunda Dose\n");
			printf("\nA primeira dose tomada foi: %s\n",aux->vacina);
		}
		else if((aux->dose2) == 1)
        {
			printf("\nJa tomou as duas doses\n");
            flag++;
        }
        if(flag==0)
        {
        	do
        	{ 
                printf("\nConfirme se a vacina foi aplicada 1-Sim 2-Nao\n");
                scanf("%d",&vac);
                if(vac==1)
                {
                    if(aux->dose1==1)
                    atualizaValor(aux, aux->nome,2, 1);
                    else 
                    atualizaValor(aux, aux->nome,1, 1);
                }
            }while(vac!=1 && vac!=2);
        }
}
void criaArquivo(TPessoa * p)
{
    FILE *arquivofinal=NULL;
	arquivofinal= fopen("unificadoZY.dat", "w");
    rewind(arquivofinal);
    while(p!=NULL)
    {
    	fprintf(arquivofinal,"\n%s %d %d %d %s",p->nome, p->idade, p->dose1, p->dose2, p->vacina);
        p = p->prox;
    }
}
int NomeVacina(char vacina[])
{
	int cont=0;
	if(strcmp(strlwr1(vacina),"astrazeneca") == 0)
		cont++;
	else if(strcmp(strlwr1(vacina),"coronavac") == 0)
		cont++;
	else if(strcmp(strlwr1(vacina),"fiucruz")== 0)
    	cont++;
	else if(strcmp(strlwr1(vacina),"janssen")== 0)
		cont++;
	else if(strcmp(strlwr1(vacina),"pfizer")== 0)
		cont++;
	else if(strcmp(strlwr1(vacina),"sputnik")== 0)
		cont++;
	return(cont);
}
void learquivo(char arquivo[])
{
	FILE *arquivofinal=NULL;
	arquivofinal= fopen(arquivo, "r");
    rewind(arquivofinal);
	char nome[20], vacina[15];
	int idade, dose1,dose2;
	char d1[4] = "Nao";
    char d2[4] = "Nao";
	if(arquivofinal==NULL) 
	{
	printf("Falha ao abrir o arquivo!");
	return;
	}
	while(feof(arquivofinal)==0)
	{
		fscanf(arquivofinal,"%s %d %d %d %s",nome,&idade, &dose1, &dose2, vacina);
		if(dose1==1)
			strcpy(d1, "Sim");

			if(dose2==1)
			strcpy(d2, "Sim");
			printf("\n%10s  Idade:%3d  1-Dose:%3s   2-Dose:%3s  %s\n",nome, idade ,d1 ,d2, vacina);
	}
	printf("\n");
}

char* strlwr1(char s[])
{
	for(int i=0;i<strlen(s);i++)
	{
		if((int)s[i]>=65 && (int)s[i]<= 90)
		s[i]=(int)s[i]+32;
	}
	return s;
}
