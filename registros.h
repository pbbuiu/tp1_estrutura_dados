#ifndef BASE_H
#define BASE_H

#define ARQUIVO_BINARIO "arq.bin"
#define TAM_DADO2

typedef struct registros{
    int chave;
    long int dado1;
    char dado2[TAM_DADO2];
} Registros;

#endif //BASE_H