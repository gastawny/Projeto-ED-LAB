#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct figurinha {
    int codSelecao;
    char selecao[20];
    int numJogador;
    char nome[20];
    struct figurinha *proxFig;
}Tfigurinha;

typedef struct selecao {
    int codSelecao;
    char selecao[20];
    Tfigurinha *inicioFig;
    struct selecao *proxSel;
}Tselecao;

typedef struct album {
    int codAlbum;
    float gastos;
    Tselecao *inicioSel;
    Tselecao *fimSel;
    struct album *proxAlb;
}Talbum;

typedef struct cabeca {
    Talbum *inicioAlb;
    Talbum *fimAlbum;
    float gastosFigRep, lucro;
    Tfigurinha *inicioFigRep;
    Tfigurinha *fimFigRep;
    int qtdAlbuns;
}Tcabeca;

void inicializacao(Tcabeca **cabeca);
void criaAlbum(Tcabeca **cabeca);
void entradaFigurinhas(Tcabeca **cabeca);
void insereFigRep(Tcabeca **cabeca, Tfigurinha *figurinha);
void alocaFigRepNoAlbum(Tcabeca **cabeca);
void insereAlbum(Tcabeca **cabeca);
void venderAlbum(Tcabeca *cabeca);
void desalocaAlbum(Tcabeca *cabeca, int k);
void desalocaSelecao(Tselecao *selecao);

int main() {
    Tcabeca *cabeca = NULL;
    int op = 1;

    while(op) {
        printf("\nDigite:\n1 - Inicializacao\n"
            "2 - Comprar novo album\n"
            "3 - Comprar pacote de figurinha\n"
            "4 - Vender album\n"
            "5 - Vender repetidas\n"
            "6 - Relatorio de gastos\n"
            "7 - Relatorio de lucros\n"
            "8 - sair\n"
        );
        scanf("%d",&op);

        switch(op) {
            case 1: inicializacao(&cabeca);break;
            case 2: insereAlbum(&cabeca);break;
            case 3: break;
            case 4: venderAlbum(cabeca);break;
            case 5: break;
            case 6: break;
            case 7: break;
            case 8: op=0;break;
            default: printf("\nDigite uma opcao valida\n");
        }
    }

    return 0;
}

void inicializacao(Tcabeca **cabeca) {
    if(*cabeca) {
        printf("\nAlbum ja foi inicializado\n");
        return;
    }
    *cabeca = (Tcabeca*)malloc(sizeof(Tcabeca));
    if(!*cabeca) {
        printf("\nErro na alocacao\n");
        return;
    }
    (*cabeca)->gastosFigRep = (*cabeca)->lucro = (*cabeca)->qtdAlbuns = 0;
    (*cabeca)->fimAlbum = (*cabeca)->inicioAlb = NULL;
    (*cabeca)->fimFigRep = (*cabeca)->inicioFigRep = NULL;
    criaAlbum(cabeca);
    (*cabeca)->inicioAlb->codAlbum = 1;
    (*cabeca)->fimAlbum = (*cabeca)->inicioAlb;
    entradaFigurinhas(cabeca);
}

void criaAlbum(Tcabeca **cabeca) {
    Talbum *album = (Talbum*)malloc(sizeof(Talbum));
    if(!album) {
        printf("\nErro na alocacao\n");
        return;
    }

    FILE *selecoes = NULL;
    selecoes = fopen("selecoes.txt","r");
    if(!selecoes) {
        printf("\nErro na abertura do arquivo\n");
        return;
    }

    Talbum *aux = album;
    Tselecao *auxSelecao = NULL;
    aux->inicioSel = aux->fimSel = NULL;
    
    if(!(*cabeca)->inicioAlb) {
        (*cabeca)->inicioAlb = album;
        aux->proxAlb = NULL;
    } else {
        (*cabeca)->fimAlbum->proxAlb = album;
        (*cabeca)->fimAlbum = album;
        aux->proxAlb = NULL;
    }
    aux->gastos = 0;

    while(!feof(selecoes)) {
        if(!aux->inicioSel) {
            aux->inicioSel = aux->fimSel = (Tselecao*)malloc(sizeof(Tselecao));
            if(!aux->inicioSel) {
                printf("\nErro na alocacao\n");
                return;
            }
        } else {
            Tselecao *novo = (Tselecao*)malloc(sizeof(Tselecao));
            if(!aux->inicioSel) {
                printf("\nErro na alocacao\n");
                return;
            }
            novo->proxSel = NULL;
            aux->fimSel->proxSel = novo;
            aux->fimSel = novo;
        }
        auxSelecao = aux->fimSel;
        auxSelecao->inicioFig = NULL;
        fscanf(selecoes,"%d%s",&(auxSelecao->codSelecao),auxSelecao->selecao);
    }
    fclose(selecoes);

    if((*cabeca)->inicioAlb->proxAlb) {
        aux = (*cabeca)->inicioAlb;
        while(aux->proxAlb != (*cabeca)->fimAlbum)
            aux = aux->proxAlb;
        (*cabeca)->fimAlbum->codAlbum = aux->codAlbum + 1;
        alocaFigRepNoAlbum(cabeca);
    }
    
    (*cabeca)->qtdAlbuns++;

    
}

void entradaFigurinhas(Tcabeca **cabeca) {
    FILE *figurinhas_entrada = NULL;
    figurinhas_entrada = fopen("figurinhas_entrada.txt","r");
    if(!figurinhas_entrada) {
        printf("\nErro na abertura do arquivo\n");
        return;
    }

    int numTime, numJogador, FLAG;
    char time[20], jogador[20];
    Tselecao *selecao = NULL;
    Tfigurinha *aux;

    while(!feof(figurinhas_entrada)) {
        FLAG = 0;
        selecao = (*cabeca)->inicioAlb->inicioSel;
        fscanf(figurinhas_entrada,"%d%s%d%s",&numTime,time,&numJogador,jogador);
        while(selecao->codSelecao != numTime)
            selecao = selecao->proxSel;
        
        Tfigurinha *figurinha = (Tfigurinha*)malloc(sizeof(Tfigurinha));
        if(!figurinha) {
            printf("\nErro na alocacao\n");
            return;
        }
        figurinha->codSelecao = numTime;
        figurinha->numJogador = numJogador;
        strcpy(figurinha->selecao,time);
        strcpy(figurinha->nome,jogador);

        if(!selecao->inicioFig) {
            figurinha->proxFig = NULL;
            selecao->inicioFig = figurinha;
        } else if(figurinha->numJogador < selecao->inicioFig->numJogador) {
            figurinha->proxFig = selecao->inicioFig;
            selecao->inicioFig = figurinha;
        } else if(figurinha->numJogador == selecao->inicioFig->numJogador) {
            insereFigRep(cabeca,figurinha);
        } else {
            aux = selecao->inicioFig;
            if(!aux->proxFig)
                FLAG = 1;
            else
                while(aux->proxFig && figurinha->numJogador > aux->proxFig->numJogador && figurinha->numJogador != aux->proxFig->numJogador) {
                    if(!aux->proxFig->proxFig) {
                        FLAG = 2;
                        break;
                    }
                    aux = aux->proxFig;
                }
            if(FLAG == 1) {
                figurinha->proxFig = aux->proxFig;
                aux->proxFig = figurinha;
            } else if(FLAG == 2) {
                figurinha->proxFig = aux->proxFig->proxFig;
                aux->proxFig->proxFig = figurinha;
            } else if(figurinha->numJogador == aux->proxFig->numJogador)
                insereFigRep(cabeca,figurinha);
            else {
                figurinha->proxFig = aux->proxFig;
                aux->proxFig = figurinha;
            }
        }
        
    }
    fclose(figurinhas_entrada);
}

void insereFigRep(Tcabeca **cabeca, Tfigurinha *figurinha) {
    if(!(*cabeca)->inicioFigRep) {
        figurinha->proxFig = NULL;
        (*cabeca)->inicioFigRep = (*cabeca)->fimFigRep = figurinha;
    }
    else {
        figurinha->proxFig = NULL;
        (*cabeca)->fimFigRep->proxFig = figurinha;
        (*cabeca)->fimFigRep = figurinha;
    }
}

void alocaFigRepNoAlbum(Tcabeca **cabeca) {
    int FLAG, count=0;
    Tfigurinha *figRep = (*cabeca)->inicioFigRep, *aux, *proxFig = figRep, *lista = NULL, *auxLista, *fimlista;
    Tselecao *selecao;
    while(proxFig) {
        FLAG = 0;
        proxFig = proxFig->proxFig;
        selecao = (*cabeca)->fimAlbum->inicioSel;

        while(figRep->codSelecao != selecao->codSelecao)
            selecao = selecao->proxSel;

        if(!selecao->inicioFig) {
            figRep->proxFig = NULL;
            selecao->inicioFig = figRep;
        } else if(figRep->numJogador < selecao->inicioFig->numJogador) {
            figRep->proxFig = selecao->inicioFig;
            selecao->inicioFig = figRep;
        } else if(figRep->numJogador == selecao->inicioFig->numJogador)
            FLAG = 3;
        else {
            aux = selecao->inicioFig;
            if(!aux->proxFig)
                FLAG = 1;
            else
                while(aux->proxFig && figRep->numJogador > aux->proxFig->numJogador && figRep->numJogador != aux->proxFig->numJogador) {
                    if(!aux->proxFig->proxFig) {
                        FLAG = 2;
                        break;
                    }
                    aux = aux->proxFig;
                }
            if(FLAG == 1) {
                figRep->proxFig = aux->proxFig;
                aux->proxFig = figRep;
            } else if(FLAG == 2) {
                figRep->proxFig = aux->proxFig->proxFig;
                aux->proxFig->proxFig = figRep;
            } else if(figRep->numJogador == aux->proxFig->numJogador)
                continue;
            else {
                figRep->proxFig = aux->proxFig;
                aux->proxFig = figRep;
            }
        }

        if(FLAG == 3) {
            fimlista = figRep;
            count++;
            if(!lista) {
                lista = figRep;
                figRep->proxFig = NULL;
            } else {
                auxLista = lista;
                lista = figRep;
                figRep->proxFig = auxLista;
            }
        }

        if(!proxFig) {
            (*cabeca)->fimFigRep = (*cabeca)->inicioFigRep = NULL; 
            if(count) {
                (*cabeca)->inicioFigRep = lista;
                (*cabeca)->fimFigRep = fimlista;
                return;
            };
            break;
        }
        figRep = proxFig;
        (*cabeca)->inicioFigRep = proxFig;
    }
}

void insereAlbum(Tcabeca **cabeca) {
    if(!*cabeca) {
        printf("\nPrograma nao inicializado\n");
        return;
    }
    criaAlbum(cabeca);
}

void venderAlbum(Tcabeca *cabeca) {
    if(!cabeca) {
        printf("\nPrograma nao inicializado\n");
        return;
    }
    if(!cabeca->inicioAlb) {
        printf("\nNao existe album para vender\n");
        return;
    }
    int k;
    Talbum *album;
    do{
        album = cabeca->inicioAlb;
        printf("\nDigite qual album deseja vender: (Aperte -1 para voltar)\n");
        for(int i=1;i<=cabeca->qtdAlbuns;i++,album=album->proxAlb)
            printf("\t%d  -  Album %d\n",i,album->codAlbum);
        scanf("%d",&k);
        if(k==-1) return;
    }while(k<1 || k>cabeca->qtdAlbuns);
    desalocaAlbum(cabeca, k);
}

void desalocaAlbum(Tcabeca *cabeca, int k) {
    Talbum *album = cabeca->inicioAlb, *auxAlb;
    k--;
    while(k) {
        album = album->proxAlb;
        k--;
    }
    Tselecao *selecao = album->inicioSel, *auxSel;
    while(selecao) {
        auxSel = selecao;
        selecao = selecao->proxSel;
        desalocaSelecao(auxSel);
        free(auxSel);
        album->inicioSel = selecao;
    }
    album->fimSel = album->inicioSel;
    cabeca->lucro = album->gastos;

    if(cabeca->inicioAlb == album)  {
        cabeca->inicioAlb = album->proxAlb;
        if(cabeca->inicioAlb == cabeca->fimAlbum)
            cabeca->fimAlbum = album->proxAlb;
    } else {
        auxAlb = cabeca->inicioAlb;
        while(auxAlb->proxAlb != album)
            auxAlb = auxAlb->proxAlb;
        if(album == cabeca->fimAlbum)
            cabeca->fimAlbum = auxAlb;
        auxAlb->proxAlb = album->proxAlb;
    }
    free(album);
    cabeca->qtdAlbuns--;
}

void desalocaSelecao(Tselecao *selecao) {
    Tfigurinha *figurinha = selecao->inicioFig, *aux;
    while(figurinha) {
        aux = figurinha;
        figurinha = figurinha->proxFig;
        free(aux);
        selecao->inicioFig = figurinha;
    }
}
