#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

int main(){



    char nomes1[3][30]={"arq_crescente.bin", "arq_decres.bin", "arq_aleatorio.bin"};
    char nomes2[3][30]={"arq_crescente.txt", "arq_decres.txt", "arq_aleatorio.txt"};
    int quantidade = 2000;
    int escolha=0;

    do{
        printf("\n===============MENU DE CRIACAO DE BINARIO ==================\n");
        printf("\n");
        printf("\n");
        
        for(int i=0; i<3; i++){
        printf("%d - Criar %s\n", i+1,nomes1[i]);
        }
        printf("4 - Criar os tres arquivos\n");
        printf("0 -Sair do Menu de Criacaoo e entra no Menu de Traducaoo\n");
        printf("Escolha uma opcao acima");

        if(scanf("%d", &escolha) != 1){//usuario digitar algo diferente de um inteiro
            while (getchar() != '\n'); 
            escolha = -1; //valor invalido cai no -1
        }
        switch(escolha){
            case 1:
                geraArquivo(nomes1[0], quantidade, escolha);
                break;
            case 2:
                geraArquivo(nomes1[1], quantidade, escolha);
                break;
            case 3:
                geraArquivo(nomes1[2], quantidade, escolha);
                break;
            case 4:
                for(int i=0; i<3; i++){
                    geraArquivo(nomes1[i], quantidade, i+1);
                }
                escolha=0;
                break;
            case 0:
                printf("Saindo...\n");
                    break;
            default:
                printf("Escolha uma opcao dentre as mostradas");
        }
    } while(escolha !=0);

    do{
        printf("\n===============MENU DE TRADUCAO DE BINARIO PARA TXT==================\n");
        printf("\n");
        printf("\n");

        for(int i=0; i<3; i++){
        printf("%d - Traduzir %s\n", i+1,nomes1[i]);
        }
        printf("4 - Traduzir os tres arquivos\n");
        printf("0 -Sair\n");
        printf("Escolha uma opcao acima");

        if(scanf("%d", &escolha) != 1){//usuario digitar algo diferente de um inteiro
            while (getchar() != '\n'); 
            escolha = -1; //valor invalido cai no -1
        }
        switch(escolha){
            case 1:
                traduz(nomes1[escolha -1], nomes2[escolha-1], quantidade);
                break;
            case 2:
                traduz(nomes1[escolha -1], nomes2[escolha-1], quantidade);
                break;
            case 3:
                traduz(nomes1[escolha -1], nomes2[escolha-1], quantidade);
                break;
            case 4:
                for(int i=0; i<3; i++){
                    traduz(nomes1[i], nomes2[i], quantidade);
                }
                break;
            case 0:
                printf("Saindo...\n");
                    break;
            default:
                printf("Escolha uma opcao dentre as mostradas");
        }
    } while(escolha !=0);
    
    return 0;



    
    return 1;
}