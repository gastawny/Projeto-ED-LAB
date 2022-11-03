#include<stdio.h>
#include<stdlib.h>

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
    Tfigurinha *fimFig;
    struct selecao *proxSel;
}Tselecao;

typedef struct album {
    int gastos;
    Tselecao *inicioSel;
    Tselecao *fimSel;
    struct album *proxAlb;
}Talbum;

typedef struct cabeca {
    Talbum *inicioAlb;
    Talbum *fimAlbum;
    int gastosFigRep;
    Tfigurinha *inicioFigRep;
    Tfigurinha *fimFigRep;
}Tcabeca;

void inicializacao(Tcabeca **cabeca);
void criaAlbum(Talbum **album);

int main() {
    Tcabeca *cabeca=NULL;
    int op=1;

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
            case 2: break;
            case 3: break;
            case 4: break;
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
    (*cabeca)->gastosFigRep = 0;
    (*cabeca)->fimAlbum = (*cabeca)->inicioAlb = NULL;
    (*cabeca)->fimFigRep = (*cabeca)->inicioFigRep = NULL;
    criaAlbum(&((*cabeca)->inicioAlb));
}

void criaAlbum(Talbum **album) {
    *album = (Talbum*)malloc(sizeof(Talbum));
    if(!*album) {
        printf("\nErro na alocacao\n");
        return;
    }

    FILE *selecoes = NULL;
    selecoes = fopen("selecoes.txt","r");
    if(!selecoes) {
        printf("\nErro na abertura do arquivo\n");
        return;
    }

    Talbum *aux = *album;
    Tselecao *auxSelecao = NULL;
    aux->inicioSel = aux->fimSel = NULL;
    aux->proxAlb = NULL;
    aux->gastos = 0;

    while(!feof(selecoes)) {
        if(!aux->inicioSel) {
            aux->inicioSel = aux->fimSel = (Tselecao*)malloc(sizeof(Tselecao));
            if(!aux->inicioSel) {
                printf("\nErro na alocacao\n");
                return;
            }
        }else{
            Tselecao *novo = (Tselecao*)malloc(sizeof(Tselecao));
            novo->proxSel = NULL;
            aux->fimSel->proxSel = novo;
            aux->fimSel = novo;
        }
        auxSelecao = aux->fimSel;
        auxSelecao->fimFig = auxSelecao->inicioFig = NULL;
        fscanf(selecoes,"%d%s",&(auxSelecao->codSelecao),auxSelecao->selecao);
    }
}
