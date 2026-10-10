#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
/* ===================CRIACAO DO VETOR DE CHAVES=========================== */

    /* teste de tamano por causa da versao do mingw 
    printf("Tamanho do registro: %lu bytes\n", (unsigned long) sizeof(Registros));
    return 0;
    */
bool geraArquivo(char *nome, int quantidade, int situacao){
    srand(time(NULL));//inicializa a semente usando relogio praaa gerar aleatorios

    int *chaves = malloc(quantidade * sizeof(int));/* malloc para criar um vetor de chaves dinamico*/
        if(chaves == NULL){
            printf("Erro ao alocar memoria\n");
            return false;
    }

    for(int i = 0 ; i < quantidade ; i++){
        if(situacao == 2){
            chaves[i] = quantidade - i;//chaves em ordem decrescente primeiro, pois o embaralhado tem que ser feito depois de criar o vetor de chaves crescente
        }
        else{
            chaves[i] = i +1 ;//chaves em ordem crescente
        }
    
    }
    if(situacao == 3){
        //embaralhar o vetor de chaves
        for(int i = quantidade-1;i>0; i--){//metodo fisher yates para embaralhar o vetor de chaves
            int j= rand() % (i+1);//sorteia uma posicao aleatoria de 0 a i

            int aux = chaves[i];//troca os elementos de posicao i e j
            chaves[i]= chaves[j];
            chaves[j]= aux;
        }
    }
    
/*=============GRAVACAO NO ARQUIVO===============*/

    FILE *arq = fopen(nome, "wb");//abre o arquivo para escrita binaria
            
            if (arq == NULL){
                printf("Erro ao abrir arquivo\n");
                free(chaves);
                return false;
            }

    Registros registro;
    memset(&registro, 0, sizeof(Registros));//zera o registro pra evitar lixo.

    for(int j = 0 ; j < TAM_DADO2 ; j++){
            registro.dado2[j] = 'A' + (rand() % 26);//gera uma letra aleatoria de A a Z
    }
    registro.dado2[TAM_DADO2 - 1] = '\0';//ultima posição vira \0 pra evitar problemas de lixo

    for(int i = 0 ; i < quantidade ; i++){
        registro.chave = chaves[i];
        registro.dado1 = rand() % 10000;//gera um numero aleatorio de 0 a 9999

        fwrite(&registro, sizeof(Registros), 1, arq);//grava o registro no arquivo
        if(ferror(arq)){
            printf("Erro ao gravar no arquivo\n");
            free(chaves);
            fclose(arq);
            return false;
        }
    }

    printf("Arquivo com '%d' registros criado com sucesso!\n", quantidade);
    fclose(arq);//fecha o arquivo
    free(chaves);
    return true;
    

    /* for(int i = 0 ; i < quantidade ; i++){//imprime o vetor de chaves
            printf("%d ", chaves[i]);
        }
        printf("\n");
    */
    
        
    /*=================FIM DO BLOCO DE CRIAÇÃO E GRAVACAO========================*/
    }
  
  
    /*====================IMPRESSAO  E LEITURA DO ARQUIVO ====================*/
bool imprimeArquivo(char *nome){  
          
    FILE *arq =fopen(nome, "rb");//abre o arquivo para leitura binaria
        if (arq == NULL){
            printf("Erro ao abrir arquivo\n");
            
            return false;
        }
        Registros registro;
        int registrosLidos = 0;

        printf("==============REGISTRIOS LIDOS DO ARQUIVO================\n");
        printf("\n");
        while(fread(&registro, sizeof(Registros), 1, arq)==1){//le o arquivo todo
            registrosLidos++;
            printf("Registro %d: chave = %3d || dado1 = %ld || dado2=  %.30s\n", registrosLidos, registro.chave, registro.dado1, registro.dado2);
        
        }
        printf("\n");
        printf("==============Total de registros lidos: %d ==============\n", registrosLidos);
        printf("==============FIM DOS REGISTROS LIDOS DO ARQUIVO ================\n");
        
        fclose(arq);
        return true;

        }
    
    bool traduz(char *nomeBin, char *nomeTxt, int quantidade){  
          
        FILE *arq =fopen(nomeBin, "rb");//abre o arquivo para leitura binaria
            if (arq == NULL){
                printf("Erro ao abrir arquivo\n");
                
                return false;
            }

        FILE *txt =fopen(nomeTxt, "w");//abre o arquivo para escrita txt
            if (txt == NULL){
                printf("Erro ao abrir arquivo\n");
                fclose(arq);
                return false;
            }
            Registros registro;
            int registrosLidos = 0;

            printf("==============REGISTRIOS ARQUIVO================\n");
            printf("\n");
           
            while(registrosLidos < quantidade && fread(&registro, sizeof(Registros), 1, arq)==1){//le o arquivo todo e vai escrevendo em um txt

                registrosLidos++;
                fprintf(txt, "Registro %d: chave = %7d || dado1 = %4ld || dado2 = %.30s \n", registrosLidos, registro.chave, registro.dado1, registro.dado2);


               
            }
            printf("==============Total de registros lidos: %d ==============\n", registrosLidos);
            printf("\n");
           
            fclose(arq);
            fclose(txt);

            return true;

    }
        