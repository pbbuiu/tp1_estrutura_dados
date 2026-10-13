#include "arvoreBinaria.h"
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
    
    char situacao[20];
    if      (strtol(argv[3], &check, 10) == 1){ strcpy(situacao, "arq_crescente.bin"); }//Crescente

    else if (strtol(argv[3], &check, 10) == 2) strcpy(situacao, "arq_decres.bin");    //Decrescente
    else                                       strcpy(situacao, "arq_aleatorio.bin"); //Aleatorio
    if (*check != '\0') return 1;
    
    int chave = strtol(argv[4], &check, 10); // <Chave>
    if (*check != '\0') return 1;

    if (argc == 6) {//Opcional de visualização do item e do processo de pesquisa
        opt = true;
    }


    Registros itemPesquisado;
    itemPesquisado.chave = chave;

    Analise a; inicializaAnalise(&a);
    
    FILE *arq = fopen(situacao, "rb");//Já faz um ponteiro para o arquivo escolhido
    if (!arq) {printf("Houve um erro na leitura do arquivo!\n"); return false;}


    if      (metodo == 1){ //Acesso sequencial indexado
        if (strcmp(situacao, "arq_crescente.bin") == 0){
            if (realizaPesquisa(&a, &itemPesquisado, quantidade, opt, arq) && opt) 
                printf("Chave: %d Dado1: %ld Dado2: %s", itemPesquisado.chave, itemPesquisado.dado1, itemPesquisado.dado2);
        }
        else{
            printf("Só é possível utilizar a pesquisa com o método de acesso sequencial indexado com vetores ordenados crescentemente nesse código\n");
            return 1;
        }
        
    }
    else if (metodo == 2){
        realizarPesquisaBinaria(&a, &itemPesquisado, quantidade, opt, arq);


    }
    else if (metodo == 3){ //Arvore B
    }
    else if (metodo == 4){
        //Testes com árvore B*


    }










    return 0;
}
