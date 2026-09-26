#ifndef BASE_H
#define BASE_H

#include <stdlib.h>

#define ARQUIVO_BINARIO "arq.bin"
#define TAM_DADO2

typedef struct registros{
    int chave;
    long int dado1;
    char dado2[TAM_DADO2];
} Registros;

typedef struct analise{
    long int transferencia;
    long int comparacoes;
    double time;
} Analise;

Analise inicializaAnalise(){
    Analise a;
    a.comparacoes = 0;
    a.transferencia = 0;
    a.time = 0;
    return a;
}

#endif //BASE_H