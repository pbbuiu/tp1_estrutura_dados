#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

//gera arquivo com a quantidade de registros e a situacçao (1 = crescente, 2 = decrescente, 3 = aleatoria)
bool geraArquivo(char *nome, int quantidade, int situacao);

//caso -P tenha sido passado como argumento, imprime o arquivo de registros
bool imprimeArquivo(char *nome);

int lePositivo(char *mensagem){
    int n;
    while(1){
        printf("%s", mensagem);
        if(scanf("%d", &n) == 1 && n > 0 )return n;
        while (getchar() != '\n'); //limpa o buffer de entrada
        printf("Digite um numero positivo: \n"); 
    }
}

bool traduz(char *nomeBin, char *nomeTxt, int quantidade);

/* ===================CRIACAO DO VETOR DE CHAVES=========================== */

    /* teste de tamano por causa da versao do mingw 
    printf("Tamanho do registro: %lu bytes\n", (unsigned long) sizeof(Registros));
    return 0;
    */
bool geraArquivo(char *nome, int quantidade, int situacao){

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
    free(chaves);// libera o vetor de chaves alocado dinamicamente
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
        if (arq == NULL){//guarda para verificar se a abertura do arquivo foi bem sucedida
            printf("Erro ao abrir arquivo\n");
            
            return false;
        }
        Registros registro;//cria uma variavel do tipo registro para ler os registros do arquivo
        int registrosLidos = 0;//contador de registros lidos

        printf("==============REGISTROS LIDOS DO ARQUIVO================\n");
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
 
int main(){//corpo da main e menu de criacao e traducao de arquivos binarios para txt

    srand(time(NULL));//inicializa a semente usando relogio praaa gerar aleatorios


    char nomes1[3][30]={"arq_crescente.bin", "arq_decres.bin", "arq_aleatorio.bin"};
    char nomes2[3][30]={"arq_crescente.txt", "arq_decres.txt", "arq_aleatorio.txt"};
    int quantidade = 0;
    int escolha=0;

    do{
        printf("\n===============MENU DE CRIACAO DE BINARIO ==================\n");
        printf("\n");
        printf("\n");
        
        for(int i=0; i<3; i++){
        printf("%d - Criar %s\n", i+1,nomes1[i]);
        }
        printf("4 - Criar os tres arquivos\n");
        printf("0 -Sair do Menu de Criacao e entra no Menu de Traducao\n");
        printf("Escolha uma opcao acima: ");

        if(scanf("%d", &escolha) != 1){//usuario digitar algo diferente de um inteiro
            while (getchar() != '\n'); 
            escolha = -1; //valor invalido cai no -1
        }

        if(escolha >= 1 && escolha <= 4){
            quantidade = lePositivo("Digite a quantidade de Registros que deseja criar: \n");
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
                printf("indo para Menu de Traducao\n");
                    break;
            default:
                printf("Opcao escolhida nao eh valida. Escolha uma opcao dentre as mostradas: ");
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
        printf("Escolha uma opcao acima: ");

        if(scanf("%d", &escolha) != 1){//usuario digitar algo diferente de um inteiro
            while (getchar() != '\n'); 
            escolha = -1; //valor invalido cai no -1
        }
        if(escolha >= 1 && escolha <= 4){
            quantidade = lePositivo("Digite a quantidade de Registros que deseja traduzir: \n");
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
                printf("Escolha uma opcao dentre as mostradas: ");
        }
    } while(escolha !=0);
    
    
    return 0;
}