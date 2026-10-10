#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ITENSPAGINA 100

typedef struct{
    int pos;        //Número da página no disco
    int chavePag;   //Primeira itemChave da página no disco
} tipoIndice;

int pesquisa (Analise *a, tipoIndice tabelaPaginas[], int tamTabela, Registros* itemPesquisado, FILE *arq, bool p);

bool realizaPesquisa(Analise *a, Registros *itemPesquisado, int qtdRegistrosArq, bool p){
    clock_gettime(CLOCK_MONOTONIC, &a->inicio); //Inicio do contador de tempo de pre-processamento
    int cont = 0;
    Registros itens[ITENSPAGINA];
    
    //Calculando o tamanho da tabela que armazenara a posição e a primeira chave da página e a alocando dinamicamente
    //{
    int tam = qtdRegistrosArq/ITENSPAGINA;
    int resto = qtdRegistrosArq % ITENSPAGINA;

    a->comparacoes++;
    if (resto != 0) tam++;
    tipoIndice *tabelaPaginas = (tipoIndice*) malloc (tam * sizeof(tipoIndice));
    //}
    
    FILE *arq = fopen("testeBin.bin", "rb");
    a->comparacoes++;
    if (!arq) {printf("Houve um erro na leitura do arquivo!\n"); return false;}
    

    while(fread(itens, sizeof(Registros), ITENSPAGINA, arq) == ITENSPAGINA){
        a->transferencia++;
        tabelaPaginas[cont].chavePag = itens[0].chave;
        tabelaPaginas[cont].pos = cont+1;
        cont++;
    }
    a->comparacoes++;
    if (resto != 0){
        a->transferencia++;
        tabelaPaginas[cont].chavePag = itens[0].chave;
        tabelaPaginas[cont].pos = tam;
    }
    clock_gettime(CLOCK_MONOTONIC, &a->fim); //Fim do contador de pre-processamento
    a->tempoTotal = (a->fim.tv_sec - a->inicio.tv_sec) + (a->fim.tv_nsec - a->inicio.tv_nsec)/ 1e9;

    printf("========ACESSO SEQUENCIAL INDEXADO========\n\n");
    printf("- Pre-processamento - \n");
    printf("Tempo: %.9lf\n", a->tempoTotal);
    printf("Numero de comparacoes: %ld\n", a->comparacoes);
    printf("Numero de transferencias: %ld\n\n", a->transferencia);

    int posItem = pesquisa(a, tabelaPaginas, tam, itemPesquisado, arq, p);
    clock_gettime(CLOCK_MONOTONIC, &a->fim);
    a->tempoTotal = (a->fim.tv_sec - a->inicio.tv_sec) + (a->fim.tv_nsec - a->inicio.tv_nsec)/ 1e9;
    printf("\n- Pesquisa - \n");
    printf("Tempo: %lf\n", a->tempoTotal);
    printf("Numero de comparacoes: %ld\n", a->comparacoes);
    printf("Numero de transferencias: %ld\n", a->transferencia);

    if (posItem !=0){
        printf("O dado se encontra na posicao: %d\n", posItem);
        return true;
    }
    printf("Não encontrou\n");
    free(tabelaPaginas);
    fclose(arq);
    return false;
}

int pesquisa (Analise *a, tipoIndice tabelaPaginas[], int tamTabela, Registros* itemPesquisado, FILE *arq, bool p) { 
    clock_gettime(CLOCK_MONOTONIC, &a->inicio);
    Registros pagina[ITENSPAGINA];
    inicializaAnalise(a);
    int qtdItens, desloc;

    int IndiceTabelaPagina=0;
    if (p) printf("\n= Comparações com as chaves armazenadas na tabela de paginas =\n");
    while(IndiceTabelaPagina < tamTabela && tabelaPaginas[IndiceTabelaPagina].chavePag <= itemPesquisado->chave){
        if (p) printf("[P:%d - T:%d]\n",itemPesquisado->chave  ,tabelaPaginas[IndiceTabelaPagina].chavePag);
        a->comparacoes++;
        IndiceTabelaPagina++;
    } 
    if (IndiceTabelaPagina == 0) return 0; //Caso a chave procurada seja menor que primeira chavePagina da tabela: A chave não existe

    a->comparacoes++;
    if (IndiceTabelaPagina < tamTabela)
        qtdItens = ITENSPAGINA;
    else{
        fseek(arq, 0, SEEK_END);
        qtdItens = (ftell(arq) / sizeof(Registros)) % ITENSPAGINA;
        if (qtdItens == 0) //Caso o arquivo dê exatemente um tamanho divisivel
            qtdItens = ITENSPAGINA;
    }

    //Calcula o deslocamento em bytes para mover o ponteiro do arquivo até o começo da pagina onde está o item
    desloc = (tabelaPaginas[IndiceTabelaPagina-1].pos - 1) * sizeof(Registros) * ITENSPAGINA; 
    fseek (arq, desloc, SEEK_SET);

    //Leitura da página na qual está a chave procurada
    fread(pagina, sizeof(Registros), qtdItens, arq); a->transferencia++;
    if (p) printf("\n= Comparações na pagina retirada do arquivo =\n");
    for (int i=0; i<qtdItens; i++){
        a->comparacoes++;
        if (p) printf("[P%d - Pa:%d]\n", itemPesquisado->chave, pagina[i].chave);
        if(pagina[i].chave == itemPesquisado->chave){
            *itemPesquisado = pagina[i];
            //Retorna a posição do dado encontrado
            return ITENSPAGINA*(tabelaPaginas[IndiceTabelaPagina-1].pos - 1) + i+1;
        }
    }
    //Se não encontrar o registro até o final do arquivo significa que ele não existe
    return 0;
}