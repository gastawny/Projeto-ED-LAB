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
void insereFig(Tcabeca *cabeca, int numSelecao, int numJogador, char tipoExtra[]);
void exibe(char s[]);

int main() {
    srand(time(NULL));
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
            default: exibe("\nDigite uma opcao valida\n");
        }
    }

    return 0;
}
// inicializa o programa, criando um album e inserindo as figurinhas do arquivo
void inicializacao(Tcabeca **cabeca) {
    if(*cabeca) {
        exibe("O programa ja foi inicializado");
        return;
    }
    *cabeca = (Tcabeca*)malloc(sizeof(Tcabeca));
    if(!*cabeca) {
        exibe("Erro na alocacao");
        return;
    }
    // inicializa a cabeca
    (*cabeca)->gastosFigRep = (*cabeca)->lucro = (*cabeca)->qtdAlbuns = 0;
    (*cabeca)->fimAlbum = (*cabeca)->inicioAlb = NULL;
    (*cabeca)->inicioFigRep = NULL;
    criaAlbum(cabeca);
    entradaFigurinhas(cabeca);
    exibe("Programa inicializado");
}
// cria um album novo
void criaAlbum(Tcabeca **cabeca) {
    Talbum *album = (Talbum*)malloc(sizeof(Talbum));
    if(!album) {
        exibe("Erro na alocacao");
        return;
    }

    FILE *selecoes = NULL;
    selecoes = fopen("selecoes.txt","r");
    if(!selecoes) {
        exibe("Erro na abertura do arquivo");
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
    // insere as selecoes no album
    while(!feof(selecoes)) {
        if(!aux->inicioSel) {
            aux->inicioSel = aux->fimSel = (Tselecao*)malloc(sizeof(Tselecao));
            if(!aux->inicioSel) {
                exibe("Erro na alocacao");
                return;
            }
        } else {
            Tselecao *novo = (Tselecao*)malloc(sizeof(Tselecao));
            if(!aux->inicioSel) {
                exibe("Erro na alocacao");
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
    // insere possiveis figurinhas repetidas no novo album
    if((*cabeca)->inicioAlb->proxAlb) {
        aux = (*cabeca)->inicioAlb;
        while(aux->proxAlb != (*cabeca)->fimAlbum)
            aux = aux->proxAlb;
        (*cabeca)->fimAlbum->codAlbum = aux->codAlbum + 1;
        alocaFigRepNoAlbum(cabeca);
    } else {
        (*cabeca)->fimAlbum = (*cabeca)->inicioAlb;
        (*cabeca)->fimAlbum->codAlbum = 1;
    }
    
    (*cabeca)->qtdAlbuns++;
}
// para ler as figurinhas do arquivo <figurinhas_entrada.txt>
void entradaFigurinhas(Tcabeca **cabeca) {
    FILE *figurinhas_entrada = NULL;
    figurinhas_entrada = fopen("figurinhas_entrada.txt","r");
    if(!figurinhas_entrada) {
        exibe("Erro na abertura do arquivo");
        return;
    }

    int numTime, numJogador;
    char spam[20];
    // pega as informacoes do arquivo e insere no album
    while(!feof(figurinhas_entrada)) {
        fscanf(figurinhas_entrada,"%d%s%d%s",&numTime,spam,&numJogador,spam);
        insereFig(*cabeca, numTime, numJogador, 0);
    }
    fclose(figurinhas_entrada);
}
// insere figurinhas na lista de repetidas
void insereFigRep(Tcabeca *cabeca, Tfigurinha *figurinha) {
    figurinha->proxFig = !cabeca->inicioFigRep ? NULL : cabeca->inicioFigRep;
    cabeca->inicioFigRep = figurinha;
    cabeca->gastosFigRep += 0.8;
}
// transefere da lista de repetidas para um album
void alocaFigRepNoAlbum(Tcabeca **cabeca) {
    int FLAG, count = 0;
    Tfigurinha *figRep = (*cabeca)->inicioFigRep, *aux, *proxFig = figRep, *lista = NULL, *auxLista;
    Tselecao *selecao;
    while(proxFig) {
        FLAG = 0;
        proxFig = proxFig->proxFig;
        selecao = (*cabeca)->fimAlbum->inicioSel;
    // encontra a selecao da figurinha
        while(figRep->codSelecao != selecao->codSelecao)
            selecao = selecao->proxSel;
    // insere de maneira ordenada a figurinha dentro da selecao
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
    // cria uma nova lista para as figurinhas que nao foram inseridas nos albuns
        if(FLAG == 3) {
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
    // atualiza os ponteiros da cabeca para essa nova lista de repetidas
        if(!proxFig) {
            (*cabeca)->inicioFigRep = NULL; 
            if(count) {
                (*cabeca)->inicioFigRep = lista;
                return;
            };
            break;
        }
        figRep = proxFig;
        (*cabeca)->inicioFigRep = proxFig;
    }
}
// chamada na main que ira criar um novo album
void insereAlbum(Tcabeca **cabeca) {
    if(!*cabeca) {
        exibe("Programa nao inicializado");
        return;
    }
    criaAlbum(cabeca);
    exibe("Album criado");
}
// chamada na main que ira vender um album
void venderAlbum(Tcabeca *cabeca) {
    if(!cabeca) {
        exibe("Programa nao inicializado");
        return;
    }
    if(!cabeca->inicioAlb) {
        exibe("Nao existe album para vender");
        return;
    }
    int k, FLAG = 0, i, aux;
    Talbum *album;
    // pergunta qual album deseja vender/desalocar
    do{
    for(album=cabeca->inicioAlb,FLAG=0;album;album=album->proxAlb)
        FLAG = album->gastos >= 524 ? FLAG + 1 : FLAG;
        printf("\n\nDigite qual album deseja vender: (Aperte -1 para voltar)\n");
        if(FLAG)
            printf("Albuns Completos:\n");
        album=cabeca->inicioAlb;
        for(i=1,k=FLAG,aux=FLAG;FLAG;album=album->proxAlb)
            if(album->gastos >= 524) {
                printf("\t%d  -  Album %d\n",i,album->codAlbum);
                i++;
                FLAG--;
            }
        if(k != cabeca->qtdAlbuns) {
            printf("Albuns Incompletos:\n"); 
            for(album = cabeca->inicioAlb;i<=cabeca->qtdAlbuns;album=album->proxAlb)
                if(album->gastos <= 524) {
                    printf("\t%d  -  Album %d\n",i,album->codAlbum);
                    i++;
                }
        }
        scanf("%d",&k);
        if(k==-1) return;
    }while(k<1 || k>i);
    if(k<=aux)
        for(i=1,aux=0,album=cabeca->inicioAlb;1;album=album->proxAlb,i++) {
            if(album->gastos >= 524) {
                aux++;
                if(aux==k) {
                    desalocaAlbum(cabeca, i);
                    break;
                }
            }
    } else {
        for(k=k-aux,i=1,aux=0,album=cabeca->inicioAlb;1;i++,album=album->proxAlb) {
            if(album->gastos <= 524) {
                aux++;
                if(aux==k) {
                    desalocaAlbum(cabeca, i);
                    break;
                }
            }
        }
    }

    exibe("Album Vendido");
}
// desaloca da memoria um album
void desalocaAlbum(Tcabeca *cabeca, int k) {
    Talbum *album = cabeca->inicioAlb, *auxAlb;
    k--;
    while(k) {
        album = album->proxAlb;
        k--;
    }
    // desaloca as selecoes
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
    // altera os ponteiros da cabeca e desaloca o album
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
// desaloca da memoria uma selecao
void desalocaSelecao(Tselecao *selecao) {
    Tfigurinha *figurinha = selecao->inicioFig, *aux;
    while(figurinha) {
        aux = figurinha;
        figurinha = figurinha->proxFig;
        free(aux);
        selecao->inicioFig = figurinha;
    }
}
// vende as figurinhas repetidas
void venderFigurinhasRepetidas(Tcabeca *cabeca) {
    if(!cabeca) {
        exibe("Programa nao inicializado");
        return;
    }
    if(!cabeca->inicioFigRep) {
        exibe("Nao possui figurinhas repetidas");
        return;
    }
    int i, k;
    Tfigurinha *figurinha = cabeca->inicioFigRep, *aux;
    do{
        printf("\nEscolha uma opcao para vender as figurinhas repetidas: (-1 para voltar)\n\n");
        for(figurinha=cabeca->inicioFigRep,i=0;figurinha;figurinha=figurinha->proxFig,i++)
            printf("%d - %d %s %d %s\n",i,figurinha->codSelecao,figurinha->selecao,figurinha->numJogador,figurinha->nome);
        printf("%d - vender todas\n",i);
        scanf("%d",&k);
        if(k == -1) return;
    }while(k<0 || k>i);
    if(k == i) {
        figurinha=cabeca->inicioFigRep;
        while(figurinha) {
            aux = figurinha;
            figurinha = figurinha->proxFig;
            free(aux);
            cabeca->inicioFigRep = figurinha;
            cabeca->lucro += 0.8;
            cabeca->gastosFigRep -= 0.8;
        }
        exibe("Figurinhas repetidas foram vendidas");
        return;
    }
    cabeca->lucro += 0.8;
    cabeca->gastosFigRep -= 0.8;
    if(!k) {
        aux = cabeca->inicioFigRep;
        cabeca->inicioFigRep = cabeca->inicioFigRep->proxFig;
        free(aux);
        return;
    }
    for(figurinha=cabeca->inicioFigRep;k-1;k--,figurinha=figurinha->proxFig);
    aux = figurinha->proxFig;
    figurinha->proxFig = figurinha->proxFig->proxFig;
    free(aux);
}
// ira mostrar os gastos
void relatorioDeGastos(Tcabeca *cabeca) {
    if(!cabeca) {
        exibe("Programa nao inicializado");
        return;
    }
    Talbum *album = cabeca->inicioAlb;
    printf(
        "------------------------------"
        "\nRelatorio de gastos:");
    for(int i=0;i<cabeca->qtdAlbuns;i++,album=album->proxAlb)
        printf("\nAlbum %d: R$ %.2f\n",album->codAlbum,album->gastos);
    printf("\nFigurinhas Repetidas: R$ %.2f\n",cabeca->gastosFigRep);
    printf("------------------------------\n");
}
// ira mostrar os lucros
void relatorioDeLucros(Tcabeca *cabeca) {
    if(!cabeca) {
        exibe("Programa nao inicializado");
        return;
    }
    printf("--------------");
    printf("\nLucro: R$ %.2f\n",cabeca->lucro);
    printf("--------------");
}
// chamada na main, ira preparar para o final do programa
void sair(Tcabeca **cabeca, int *op) {
    *op = 0;
    if(!*cabeca) {
        exibe("Programa Finalizado");
        return;
    }
    float despesas = (*cabeca)->gastosFigRep;
    int cod, asciiTable = 48;
    char nomAlbum[20], aux[10] = "Album_", aux2[2];
    Talbum *album = (*cabeca)->inicioAlb;
    FILE *file = NULL;
    // cria um novo arquivo para cada album
    while(album) {
        cod = album->codAlbum;
        *aux2 = cod + asciiTable;
        *(aux2+1) = '\0';
        strcpy(nomAlbum,aux);
        strcat(nomAlbum,aux2);
        strcat(nomAlbum,".txt");
        file = fopen(nomAlbum,"w");
        if(!file) {
        exibe("Erro ao criar o arquivo");
        return;
        }
        imprimeAlbum(album,file);
        despesas += album->gastos;
        album = album->proxAlb;
        fclose(file);
    }
    // cria um arqquivo para lucros e despesas
    file = fopen("lucros_e_despesas.txt","w");
    if(!file) {
        exibe("Erro ao criar o arquivo");
        return;
    }
    fprintf(file,"Lucros: R$ %.2lf\n\nDespesas: R$ %.2lf",(*cabeca)->lucro,despesas);
    fclose(file);
    // cria um arqquivo para figurinhas repetidas
    file = fopen("figurinhas_repetidas.txt","w");
    if(!file) {
        exibe("Erro ao criar o arquivo");
        return;
    }
    fprintf(file,"Figurinhas Repetidas:\n");
    for(Tfigurinha *figRep = (*cabeca)->inicioFigRep;figRep;figRep = figRep->proxFig)
        fprintf(file,"\n%d %s %d %s",figRep->codSelecao, figRep->selecao, figRep->numJogador, figRep->nome);
    fclose(file);
    // desaloca todos os albuns
    for(int i=(*cabeca)->qtdAlbuns;i>0;i--)
        desalocaAlbum(*cabeca,i);
    // desaloca as figurinhas repetidas
    for(Tfigurinha *aux;(*cabeca)->inicioFigRep;) {
            aux = (*cabeca)->inicioFigRep;
            (*cabeca)->inicioFigRep = (*cabeca)->inicioFigRep->proxFig;
            free(aux);
            (*cabeca)->lucro += 0.8;
            (*cabeca)->gastosFigRep -= 0.8;
        }
    free(*cabeca);
    *cabeca = NULL;
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    exibe("Programa Finalizado");
    printf("\n");
}
// imprime um album em um arquivo
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
// compra um novo pacote de figurinhas
void comprarPacote(Tcabeca *cabeca) {
    if(!cabeca) {
        exibe("Programa nao inicializado");
        return;
    }
    Talbum *album = cabeca->inicioAlb;
    Tselecao *selecao;
    Tfigurinha *figurinha, *aux;
    int numSelecao, numJogador, extra, FLAG = 0;
    // gera 5 figurinhas e aloca elas
    for(int i=0;i<5;i++) {
        numSelecao = rand() % 32 + 1;
        numJogador = rand() % 19 + 1;
        insereFig(cabeca, numSelecao, numJogador, 0);
    }
    // gera uma possivel figurinhas extra
    extra = rand() % 190;
    if(!extra && !FLAG) {
        FLAG = 1;
        extra = rand() % 32 + 1;
        insereFig(cabeca, extra, 20, "Bordo");
    }
    extra = rand() % 317;
    if(!extra && !FLAG) {
        FLAG = 1;
        extra = rand() % 32 + 1;
        insereFig(cabeca, extra, 20, "Bronze");
    }
    extra = rand() % 950;
    if(!extra && !FLAG) {
        FLAG = 1;
        extra = rand() % 32 + 1;
        insereFig(cabeca, extra, 20, "Prata");
    }
    extra = rand() % 1900;
    if(!extra && !FLAG) {
        FLAG = 1;
        extra = rand() % 32 + 1;
        insereFig(cabeca, extra, 20, "Ouro");
    }
    exibe("Pacote Comprado");
}
// insere uma figurinha em qualquer album
void insereFig(Tcabeca *cabeca, int numSelecao, int numJogador, char tipoExtra[]) {
    int auxSelecao, auxJogador, FLAG;
    char jogador[20];
    Talbum *album = cabeca->inicioAlb;
    Tselecao *selecao;
    FILE *figurinhas_total = NULL;
    figurinhas_total = fopen("figurinhas_total.txt","r");
    while(numSelecao  != auxSelecao)
        fscanf(figurinhas_total,"%d%d%s",&auxSelecao,&auxJogador,jogador);
    while(numJogador != auxJogador)
        fscanf(figurinhas_total,"%d%d%s",&auxSelecao,&auxJogador,jogador);
    fclose(figurinhas_total);

    Tfigurinha *figurinha = (Tfigurinha*)malloc(sizeof(Tfigurinha)), *aux;
    if(!figurinha) {
        exibe("Erro na alocacao");
        return;
    }
    // insere os dados na figurinha
    figurinha->codSelecao = auxSelecao;
    figurinha->numJogador = auxJogador;
    strcpy(figurinha->nome,jogador);

    if(!album) {
        FILE *selecoes = fopen("selecoes.txt","r");
        if(!selecoes) {
            exibe("Erro na abertura do arquivo");
            return;
        }
        char auxNomeSelecao[20];
        int auxNumeroSelecao;
        while(auxNumeroSelecao != numSelecao)
            fscanf(selecoes,"%d%s",&auxNumeroSelecao,auxNomeSelecao);
        strcpy(figurinha->selecao,auxNomeSelecao);
    }
    
    if(tipoExtra)
        strcat(figurinha->nome, tipoExtra);

    if(!cabeca->inicioAlb)
        insereFigRep(cabeca,figurinha);
    // insere a figurinha no album
    for(FLAG=0;album;album=album->proxAlb) {
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
            if(!album->proxAlb) {
                insereFigRep(cabeca,figurinha);
                FLAG = 3;
            }
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
                if(!album->proxAlb) {
                    insereFigRep(cabeca,figurinha);
                    FLAG = 3;
                }
            } else {
                figurinha->proxFig = aux->proxFig;
                aux->proxFig = figurinha;
            }
        }
        if(FLAG != 3)
            album->gastos += 0.8;
        if(tipoExtra)
            album->gastos -= 0.8;
    }
}
// "printf mais bonito"
void exibe(char s[]) {
    int n = strlen(s);
    printf("\n");
    for(int i=0;i<n;i++)
        printf("-");
    printf("\n%s\n",s);
    for(int i=0;i<n;i++)
        printf("-");
    printf("\n");
}
