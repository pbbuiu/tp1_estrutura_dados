#include "registros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h> //teste

int main(int argc, char *argv[]){

    // //Verificação dos argumentos da execução

    // if (argc < 5){
    //     printf("Comando no console incompleto\n");
    //     return 1;

    bool opt;
    char *check;

    //Transforma os argumentos da execução do programa em seus respectivos tipos

    int metodo     = strtol(argv[1], &check, 10); // <Metodo>
    if (*check != '\0') return 1;
    
    int quantidade = strtol(argv[2], &check, 10); // <Quantidade>
    if (*check != '\0') return 1;
    
    int situacao   = strtol(argv[3], &check, 10); // <Situacao>
    if (*check != '\0') return 1;
    
    int chave = strtol(argv[4], &check, 10); // <Chave>
    if (*check != '\0') return 1;

    //Opção de apresentação das chaves de pesquisa dos registros do arquivo considerado
    if (argc == 6) { //adicionado pq qualquer coisa escrita como quinto argumento estava ativando a opção de mostrar as chaves pesquisadas na tela
        if (strcmp(argv[5], "-P") == 0) opt = true;
        else { printf("Argumento opcional invalido: %s\n", argv[5]); return 1; }
    }


    

    Registros itemPesquisado;
    itemPesquisado.chave = chave;

    if      (strcmp(argv[1], "1")){
        //Testes com o método de acesso sequencial indexado


    }
    else if (strcmp(argv[1], "2")){
        //Testes com árvore binária adequada à memória externa


    }
    else if (strcmp(argv[1], "2")){
        //Testes com árvore B


    }
    else if (strcmp(argv[1], "2")){
        //Testes com árvore B*


    }










    return 0;
}
