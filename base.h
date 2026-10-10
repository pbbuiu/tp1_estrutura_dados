#ifndef BASE_H
#define BASE_H

#include <time.h>
#include <stdlib.h>

#define ARQUIVO_BINARIO "arq.bin"
#define TAM_DADO2 5000

typedef struct registros{
    int chave;
    long int dado1;
    char dado2[TAM_DADO2];
} Registros;

typedef struct analise{
    long int transferencia;
    long int comparacoes;
    struct timespec inicio;
    struct timespec fim;
    double tempoTotal;
} Analise;

void inicializaAnalise(Analise *a){
    a->comparacoes = 0;
    a->transferencia = 0;
    a->tempoTotal = 0;
}

#endif //BASE_H