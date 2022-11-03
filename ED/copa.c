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
            case 1: break;
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
