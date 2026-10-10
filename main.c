#include "acesso_sequencial.h"
#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

int main(int argc, char *argv[]){
    //Verificação dos argumentos da execução

    if (argc < 5){printf("Comando no console incompleto\n");  return 1;}
    bool opt;
    char *check;

    //Transforma os argumentos da execução do programa em seus respectivos tipos

    int metodo     = strtol(argv[1], &check, 10); // <Metodo> Acesso[1] Binaria[2] B[3] B*[4]
    if (*check != '\0') return 1; 
    
    int quantidade = strtol(argv[2], &check, 10); // <Quantidade>
    if (*check != '\0') return 1;
    
    int situacao   = strtol(argv[3], &check, 10); // <Situacao>  Crescente[1] Decrescente[2] Aleatorio[3]
    if (*check != '\0') return 1;
    
    int chave = strtol(argv[4], &check, 10); // <Chave>
    if (*check != '\0') return 1;

    //Opção de apresentação das chaves de pesquisa dos registros do arquivo considerado
    if (argc == 6) {
        opt = false;
        if (strcmp(argv[5], "-P") == 0) opt = true;
        else { printf("Argumento opcional invalido: %s\n", argv[5]); return 1; }
    }


    Registros itemPesquisado;
    itemPesquisado.chave = chave;
    
    Analise a; inicializaAnalise(&a);
    

    if      (metodo == 1){ //Acesso sequencial indexado
        if (situacao == 1)
            realizaPesquisa(&a, &itemPesquisado, quantidade, opt);
        else{
            printf("Só é possível utilizar a pesquisa com o método de acesso sequencial indexado com vetores ordenados crescentemente nesse código\n");
            return 1;
        }
        
    }
    else if (metodo == 2){
        realizaPesquisaBinaria(&a, &itemPesquisado, quantidade, opt);


    }
    else if (metodo == 3){
        //Testes com árvore B


    }
    else if (metodo == 4){
        //Testes com árvore B*


    }










    return 0;
}
