#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ITENSPAGINA 10

typedef struct{
    int pos;        //Número da página no disco
    int chavePag;   //Primeira itemChave da página no disco
} tipoIndice;

bool realizaPesquisa(Analise *a, Registros *itemPesquisado, int qtdRegistrosArq, OrdemArquivo situacao, bool p){
    int cont = 0;
    Registros itens[ITENSPAGINA];

    //Calculando o tamanho da tabela que armazenara a posição e a primeira chave da página e a alocando dinamicamente
    //{
    int tam = qtdRegistrosArq/ITENSPAGINA;
    int resto = qtdRegistrosArq % ITENSPAGINA;
    if (resto != 0) tam++;
    tipoIndice *tabelaPaginas = (tipoIndice*) malloc (tam * sizeof(tipoIndice));
    //}
    
    FILE *arq = fopen("testeBin.bin", "rb");
    if (!arq) {printf("Houve um erro na leitura do arquivo!\n"); return false;}
    
    while(fread(itens, sizeof(Registros), ITENSPAGINA, arq) == ITENSPAGINA){
        tabelaPaginas[cont].chavePag = itens[0].chave;
        tabelaPaginas[cont].pos = cont+1;
        cont++;
    }
    if (resto != 0){
        tabelaPaginas[cont].chavePag = itens[0].chave;
        tabelaPaginas[cont].pos = tam;
    }
    
    if (pesquisa(tabelaPaginas, qtdRegistrosArq, itemPesquisado, arq))
        return false;
    else
        return true;
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