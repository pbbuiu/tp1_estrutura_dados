#include "base.h"
#include <stdio.h>
#include <stdlib.h>

#define ITENSPAGINA 100

typedef struct{
    int pos;        //Número da página no disco
    int chavePag;   //Primeira itemChave da página no disco
} tipoIndice;

bool realizaPesquisa(Analise *a, Registros *itemPesquisado, int qtdRegistrosArq, bool p){
    //Calculando o tamanho da tabela que armazenara a posição e a primeira chave da página e a alocando dinamicamente
    //{
    int tam = qtdRegistrosArq/ITENSPAGINA;
    int resto = qtdRegistrosArq % ITENSPAGINA;
    if (resto != 0) tam++;

    tipoIndice *tabelaPaginas = (tipoIndice*) malloc (tam * sizeof(tipoIndice));
    if (!tabelaPaginas){
        printf("Problema na alocação dinâmica: tabelaPaginas");
        return false;
    }
    //}

    Registros itens[ITENSPAGINA];

    FILE *arq = fopen("arq.bin", "rb");
    if (!arq){
        printf("Houve um erro na leitura do arquivo!\n");
        return false;
    }

    while(fread())
}

int pesquisa (tipoIndice tabelaPaginas[], int qtdRegistros, Registros* item, FILE *arq) { 
    Registros pagina[ITENSPAGINA];
    int i, qtdItens, desloc;

    i=0;
    while(i < tam && tabelaPaginas[i].chavePag <= item[0].chave) i++;

    if (i == 0) return 0; //Caso a chave procurada seja menor que primeira chavePagina da tabela
    else{
        if (i < tam)
            qtdItens = ITENSPAGINA;
        else{
            fseek(arq, 0, SEEK_END);
            qtdItens = (ftell(arq) / sizeof(Registros)) % ITENSPAGINA;
            if (!qtdItens)
                qtdItens = ITENSPAGINA;
        }
    }

    desloc = tabelaPaginas[i=1].pos - 1 * sizeof(Registros) * ITENSPAGINA;
    
    
}