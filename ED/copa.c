#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

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
    double gastos;
    Tselecao *inicioSel;
    Tselecao *fimSel;
    struct album *proxAlb;
}Talbum;

typedef struct cabeca {
    Talbum *inicioAlb;
    Talbum *fimAlbum;
    double gastosFigRep, lucro;
    Tfigurinha *inicioFigRep;
    Tfigurinha *fimFigRep;
    int qtdAlbuns;
}Tcabeca;

void inicializacao(Tcabeca **cabeca);
void criaAlbum(Tcabeca **cabeca);
void entradaFigurinhas(Tcabeca **cabeca);
void insereFigRep(Tcabeca *cabeca, Tfigurinha *figurinha);
void alocaFigRepNoAlbum(Tcabeca **cabeca);
void insereAlbum(Tcabeca **cabeca);
void venderAlbum(Tcabeca *cabeca);
void desalocaAlbum(Tcabeca *cabeca, int k);
void desalocaSelecao(Tselecao *selecao);
void venderFigurinhasRepetidas(Tcabeca *cabeca);
void relatorioDeGastos(Tcabeca *cabeca);
void relatorioDeLucros(Tcabeca *cabeca);
void sair(Tcabeca **cabeca, int *op);
void imprimeAlbum(Talbum *album, FILE *file);
void comprarPacote(Tcabeca *cabeca);

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
            case 3: comprarPacote(cabeca);break;
            case 4: venderAlbum(cabeca);break;
            case 5: venderFigurinhasRepetidas(cabeca);break;
            case 6: relatorioDeGastos(cabeca);break;
            case 7: relatorioDeLucros(cabeca);break;
            case 8: sair(&cabeca,&op);break;
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
    aux->gastos = 12;

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
            insereFigRep(*cabeca,figurinha);
            FLAG = 3;
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
            } else if(figurinha->numJogador == aux->proxFig->numJogador) {
                insereFigRep(*cabeca,figurinha);
                FLAG = 3;
            }
            else {
                figurinha->proxFig = aux->proxFig;
                aux->proxFig = figurinha;
            }
        }
        if(FLAG != 3)
            (*cabeca)->inicioAlb->gastos += 0.8;
    }
    fclose(figurinhas_entrada);
}

void insereFigRep(Tcabeca *cabeca, Tfigurinha *figurinha) {
    if(!cabeca->inicioFigRep) {
        figurinha->proxFig = NULL;
        cabeca->inicioFigRep = cabeca->fimFigRep = figurinha;
    } else {
        figurinha->proxFig = NULL;
        cabeca->fimFigRep->proxFig = figurinha;
        cabeca->fimFigRep = figurinha;
    }
    cabeca->gastosFigRep += 0.8;
}

void alocaFigRepNoAlbum(Tcabeca **cabeca) {
    int FLAG, count = 0;
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
            count = 1;
            if(!lista) {
                lista = figRep;
                figRep->proxFig = NULL;
            } else {
                auxLista = lista;
                lista = figRep;
                figRep->proxFig = auxLista;
            }
        } else {
            (*cabeca)->gastosFigRep -= 0.8;
            (*cabeca)->fimAlbum->gastos += 0.8;
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

    if(cabeca->inicioAlb == album) {
        if(cabeca->inicioAlb == cabeca->fimAlbum)
            cabeca->fimAlbum = album->proxAlb = cabeca->inicioAlb = NULL;
        else
            cabeca->inicioAlb = album->proxAlb;
    }
    else {
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

void venderFigurinhasRepetidas(Tcabeca *cabeca) {
    if(!cabeca) {
        printf("\nPrograma nao inicializado\n");
        return;
    }
    Tfigurinha *figurinha = cabeca->inicioFigRep, *aux;
    while(figurinha) {
        aux = figurinha;
        figurinha = figurinha->proxFig;
        free(aux);
        cabeca->inicioFigRep = figurinha;
        cabeca->lucro += 0.8;
        cabeca->gastosFigRep -= 0.8;
    }
    cabeca->fimFigRep = NULL;
}

void relatorioDeGastos(Tcabeca *cabeca) {
    if(!cabeca) {
        printf("\nPrograma nao inicializado\n");
        return;
    }
    Talbum *album = cabeca->inicioAlb;
    printf("\nRelatorio de gastos:\n");
    for(int i=0;i<cabeca->qtdAlbuns;i++,album=album->proxAlb)
        printf("Album %d: R$ %.2f\n",album->codAlbum,album->gastos);
    printf("Figurinhas Repetidas: R$ %.2f\n",cabeca->gastosFigRep);
}

void relatorioDeLucros(Tcabeca *cabeca) {
    if(!cabeca) {
        printf("\nPrograma nao inicializado\n");
        return;
    }
    printf("\nLucro: R$ %.2f\n",cabeca->lucro);
}

void sair(Tcabeca **cabeca, int *op) {
    *op = 0;
    if(!*cabeca) return;
    float despesas = (*cabeca)->gastosFigRep;
    int cod, asciiTable = 48;
    char nomAlbum[20], aux[10] = "Album_", aux2[2];
    Talbum *album = (*cabeca)->inicioAlb;
    FILE *file = NULL;

    while(album) {
        cod = album->codAlbum;
        *aux2 = cod + asciiTable;
        *(aux2+1) = '\0';
        strcpy(nomAlbum,aux);
        strcat(nomAlbum,aux2);
        strcat(nomAlbum,".txt");
        file = fopen(nomAlbum,"w");
        if(!file) {
        printf("\nErro ao criar o arquivo");
        return;
        }
        imprimeAlbum(album,file);
        despesas += album->gastos;
        album = album->proxAlb;
        fclose(file);
    }

    file = fopen("lucros_e_despesas.txt","w");
    if(!file) {
        printf("\nErro ao criar o arquivo");
        return;
    }
    fprintf(file,"Lucros: R$ %.2lf\n\nDespesas: R$ %.2lf",(*cabeca)->lucro,despesas);
    fclose(file);

    file = fopen("figurinhas_repetidas.txt","w");
    if(!file) {
        printf("\nErro ao criar o arquivo");
        return;
    }
    fprintf(file,"Figurinhas Repetidas:\n");
    for(Tfigurinha *figRep = (*cabeca)->inicioFigRep;figRep;figRep = figRep->proxFig)
        fprintf(file,"\n%d %s %d %s",figRep->codSelecao, figRep->selecao, figRep->numJogador, figRep->nome);
    fclose(file);

    for(int i=(*cabeca)->qtdAlbuns;i>0;i--)
        desalocaAlbum(*cabeca,i);

    venderFigurinhasRepetidas(*cabeca);
    free(*cabeca);
    *cabeca = NULL;
}

void imprimeAlbum(Talbum *album, FILE *file) {
    Tselecao *selecao = album->inicioSel;
    Tfigurinha *fig;
    fprintf(file,"Album %d:\n",album->codAlbum);
    while(selecao) {
        for(fig = selecao->inicioFig; fig; fig = fig->proxFig)
            fprintf(file,"\n%d %s %d %s",fig->codSelecao, fig->selecao, fig->numJogador, fig->nome);
        selecao = selecao->proxSel;
    }
}

void comprarPacote(Tcabeca *cabeca) {
    srand(time(NULL));
    Talbum *album = cabeca->inicioAlb;
    Tselecao *selecao;
    Tfigurinha *figurinha, *aux;
    int numSelecao, numJogador, extra, auxSelecao, auxJogador, FLAG;
    char nomeSelecao[20], jogador[20];
    FILE *figurinhas_total = NULL;
    figurinhas_total = fopen("figurinhas_total.txt","r");
    if(!figurinhas_total) {
        printf("\nErro na abertura do arquivo\n");
        return;
    }
    for(int i=0;i<5;i++) {
        rewind(figurinhas_total);
        numSelecao = rand() % 20 + 1;
        numJogador = rand() % 19 + 1;
        auxSelecao = auxJogador = 0;
        while(numSelecao  != auxSelecao)
            fscanf(figurinhas_total,"%d%d%s",&auxSelecao,&auxJogador,jogador);
        while(numJogador != auxJogador)
            fscanf(figurinhas_total,"%d%d%s",&auxSelecao,&auxJogador,jogador);

        figurinha = (Tfigurinha*)malloc(sizeof(Tfigurinha));
        if(!figurinha) {
            printf("\nErro na alocacao\n");
            return;
        }
        figurinha->codSelecao = auxSelecao;
        figurinha->numJogador = auxJogador;
        strcpy(figurinha->nome,jogador);

        if(!cabeca->inicioAlb)
            insereFigRep(cabeca,figurinha);
        for(album=cabeca->inicioAlb,FLAG=0;album;album=album->proxAlb) {
            selecao = album->inicioSel;
            while(selecao->codSelecao != numSelecao)
                selecao = selecao->proxSel;
            strcpy(figurinha->selecao,selecao->selecao);
            if(!selecao->inicioFig) {
                figurinha->proxFig = NULL;
                selecao->inicioFig = figurinha;
            } else if(figurinha->numJogador < selecao->inicioFig->numJogador) {
                figurinha->proxFig = selecao->inicioFig;
                selecao->inicioFig = figurinha;
            } else if(figurinha->numJogador == selecao->inicioFig->numJogador) {
                insereFigRep(cabeca,figurinha);
                FLAG = 3;
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
                } else if(figurinha->numJogador == aux->proxFig->numJogador) {
                    insereFigRep(cabeca,figurinha);
                    FLAG = 3;
                }
                else {
                    figurinha->proxFig = aux->proxFig;
                    aux->proxFig = figurinha;
                }
            }
            if(FLAG != 3)
                album->gastos += 0.8;
            
        }
    }
}