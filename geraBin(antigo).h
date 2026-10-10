#ifndef GERABIN_H
#define GERABIN_H

#include <stdbool.h>

//gera arquivo com a quantidade de registros e a situacçao (1 = crescente, 2 = decrescente, 3 = aleatoria)
bool geraArquivo(char *nome, int quantidade, int situacao);

//caso -P tenha sido passado como argumento, imprime o arquivo de registros
bool imprimeArquivo(char *nome);

#endif // GERABIN_H
