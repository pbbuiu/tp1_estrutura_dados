#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

typedef struct{
    Registros registro;
    int esq;
    int dir;
} Item;

// Funcao responsavel por adicionar um item a arvore
void inserir(FILE* arvore, FILE* registrosBinarios, Analise* analisador){ 
    
    // Transferencia de um registro do arquivo binario de registros para uma variavel na memoria interna
    Registros novoRegistro;
    fread(&novoRegistro, sizeof(Registros), 1, registrosBinarios);
    analisador->transferencia++; // +1 tranferencia

    // Cria o novo item e faz as devidas associacoes
    Item novoItem;
    novoItem.registro.chave = novoRegistro.chave;
    novoItem.registro.dado1 = novoRegistro.dado1;
    strcpy(novoItem.registro.dado2, novoRegistro.dado2);
    novoItem.esq = -1;
    novoItem.dir = -1;

    // Insercao do primeiro item, logo eh executado apenas uma vez quando a arvore esta vazia
    if(ftell(arvore) == 0){
        fwrite(&novoItem, sizeof(Item), 1, arvore); // Escreve o primeiro item
        analisador->transferencia++; 
        return;
    }
        
    fwrite(&novoItem, sizeof(Item), 1, arvore); // Escreve o item no final da arvore
    analisador->transferencia++; 

    int posicaoNovoItem = ftell(arvore)/sizeof(Item) - 1; // Salva a posicao do item que acabou de ser adicionado  
    int posicaoParaAttArvore; // Variavel responsavel por guardar a posicao do ultimo item vizitado pelo ponteiro, importante para atualizacao do esq e dir

    fseek(arvore, 0, SEEK_SET); // Voltamos para o inicio da arvore para fazer a navegacao por ela

    Item ItemAtual; // Variavel que recebe o item lido na arvore, usada para as devidas comparacoes
    while(1){
        posicaoParaAttArvore = ftell(arvore)/sizeof(Item); // Guarda a posicao do ultimo item lido

        fread(&ItemAtual, sizeof(Item), 1, arvore);
        analisador->transferencia++; 

        if(novoItem.registro.chave < ItemAtual.registro.chave){
            analisador->comparacoes++;

            // Caso o campo esq seja -1, quer dizer que é um "no" sem filhos, logo é aqui que vamos atualizar o endereco
            if(ItemAtual.esq == -1){ 
                analisador->comparacoes++;
                ItemAtual.esq = posicaoNovoItem; // Variavel na memoria interna recebe a posicao do novo item adicionado 

                fseek(arvore, posicaoParaAttArvore*sizeof(Item), SEEK_SET); // Vamos para a posicao do item que vamos atualizar na arvore binaria
                fwrite(&ItemAtual, sizeof(Item), 1, arvore);
                analisador->transferencia++; 

                fseek(arvore, 0, SEEK_END); // Jogamos o ponteiro no final para adicionar o proximo item caso exista

                break;
            }

            // So caimos nesse else caso a item atual possua filho (dir ou esq != -1), logo vamos para a posicao desse filho
            else{
                fseek(arvore, ItemAtual.esq*sizeof(Item), SEEK_SET); // Ponteiro vai do inicio até a posicao do proximo item
            }
        }

        else if(novoItem.registro.chave > ItemAtual.registro.chave){
             analisador->comparacoes++; // +1 comparacao

            // Caso o campo dir seja -1, quer dizer que é um "no" sem filhos, logo é aqui que vamos atualizar o endereco
            if(ItemAtual.dir == -1){
                analisador->comparacoes++; // +1 comparacao
                ItemAtual.dir = posicaoNovoItem; // Variavel na memoria interna recebe a posicao do novo item adicionado 

                fseek(arvore, posicaoParaAttArvore*sizeof(Item), SEEK_SET);// Vamos para a posicao do item que vamos atualizar na arvore binaria
                fwrite(&ItemAtual, sizeof(Item), 1, arvore);
                analisador->transferencia++; // +1 tranferencia

                fseek(arvore, 0, SEEK_END); // Jogamos o ponteiro no final para adicionar o proximo item caso exista

                break;
            }

            // So caimos nesse else caso a item atual possua filho (dir ou esq != -1), logo vamos para a posicao desse filho
            else{
                fseek(arvore, ItemAtual.dir*sizeof(Item), SEEK_SET); // Ponteiro vai do inicio até a posicao do proximo item
            }
        }

        else{
            printf("ERROR, nao pode haver chaves iguais\n"); 
            return;
        }
    }

}

// Funcao responsavel por navegar pela arvore para encontrar um item
void pesquisar(int chave, Analise* analisador, bool printer){
    FILE* arvore = fopen("arvoreBinaria.bin", "rb"); 
    if(arvore == NULL){
        printf("Erro ao abrir o arvore Binaria para fazer a pesquisa\n");
        return;
    }

    Item itemAtual; 
    while(1){
        fread(&itemAtual, sizeof(Item), 1, arvore); // Le o item apontado pelo ponteiro e joga ele na variavel 
        analisador->transferencia++; // +1 transferencia

        // Caso o valor procurado seja menor do o valor atual e diferente de -1, vamos para o item na posicao guardada no esq
        if(chave < itemAtual.registro.chave && itemAtual.esq != -1){   
            analisador->comparacoes++; // +1 comparacao 
            if(printer) printf("Comparacao atual: [%d <-> %d] --- Transferencias: %ld Compacoes: %ld\n", itemAtual.registro.chave, chave, analisador->transferencia, analisador->comparacoes);
            fseek(arvore, itemAtual.esq*sizeof(Item), SEEK_SET); // Vai para posicao mostrada pelo esq
        }
        
        // Caso o valor procurado seja maior do o valor atual e diferente de -1, vamos para o item na posicao guardada no dir
        else if(chave > itemAtual.registro.chave && itemAtual.dir != -1){
            analisador->comparacoes++; // +1 comparacao 
            if(printer) printf("Comparacao atual: [%d <-> %d] --- Transferencias: %ld Compacoes: %ld\n", itemAtual.registro.chave, chave, analisador->transferencia, analisador->comparacoes);
            fseek(arvore, itemAtual.dir*sizeof(Item), SEEK_SET); // Vai para a posicao mostrada pelo dir
        }

        // Achamos o item
        else if(chave == itemAtual.registro.chave){
            analisador->comparacoes++; 
            if(printer) printf("Comparacao atual: [%d <-> %d] --- Transferencias: %ld Compacoes: %ld\n", itemAtual.registro.chave, chave, analisador->transferencia, analisador->comparacoes);
            printf("\nRegistro encontrado na posicao %ld da arvore binaria\n", ftell(arvore)/sizeof(Item));
            if (printer) printf("Chave: %d\nDado1: %ld\nDado2: %s.- \n", chave, itemAtual.registro.dado1, itemAtual.registro.dado2);
            break;
        }

        // Nao achamos o item
        else{
            printf("Item nao existe na arvore\n");
            break;
        }
    }

    fclose(arvore);
}   

bool realizarPesquisaBinaria(Analise* analisador, Registros* itemPesquisado, int quantidade, char* situacao, bool printer, FILE* registrosBinarios) {
    // Toda essa parte diz respeito apenas a selecao do tipo de arquivo que sera usado pra construcao da arvore
    printf("\n======== ARVORE BINARIA MEMORIA EXTERNA ========\n");

    FILE* arvore = fopen("arvoreBinaria.bin", "wb+"); 
    if(arvore == NULL){printf("Erro ao criar o arquivo arvoreBinaria.bin\n"); return 0;}

    printf("\n- PRE-PROCESSAMENTO -\n\n");

    // Insere todos os valores do vetor na arvore binaria
    clock_gettime(CLOCK_MONOTONIC, &analisador->inicio);
    for(int i = 0; i < quantidade; i++){
        inserir(arvore, registrosBinarios, analisador);
    }
    clock_gettime(CLOCK_MONOTONIC, &analisador->fim);
    analisador->tempoTotal = (analisador->fim.tv_sec - analisador->inicio.tv_sec) + (analisador->fim.tv_nsec - analisador->inicio.tv_nsec) / 1e9;

    // Imprime as analises da montagem da arvore
    printf("- Criacao da arvore binaria:\n");
    printf("Tempo: %.9lf\nNumero de comparacoes: %ld\nNumero de transferencias: %ld\n\n", analisador->tempoTotal, analisador->comparacoes, analisador->transferencia);
    inicializaAnalise(analisador); // Reseta os valores do analisador por que a arvore foi montada 

    clock_gettime(CLOCK_MONOTONIC, &analisador->inicio);
    pesquisar(itemPesquisado->chave, analisador, printer);
    clock_gettime(CLOCK_MONOTONIC, &analisador->fim);
    analisador->tempoTotal = (analisador->fim.tv_sec - analisador->inicio.tv_sec) + (analisador->fim.tv_nsec - analisador->inicio.tv_nsec) / 1e9;

    // Imprime as analises da pesquisa
    printf("\n- Pesquisa -\n");
    printf("Tempo: %lf\nNumero de comparacoes: %ld\nNumero de transferencias: %ld\n\n", analisador->tempoTotal, analisador->comparacoes, analisador->transferencia);

    // Criacao de um arquivo txt que representa a arvore binaria
    FILE* txt;
    if((txt = fopen("arvoreBinaria.txt", "w")) != NULL){
        Item conversor;
        fseek(arvore, 0, SEEK_SET);
        for(int i = 0; i < quantidade; i++){
            fread(&conversor, sizeof(Item), 1, arvore);
            fprintf(txt, "%d %d %d\n", conversor.esq, conversor.registro.chave, conversor.dir);
        }
        fclose(txt);
    }
    else printf("Erro ao criar o arquivo txt da arvore binaria\n");

    fclose(arvore);

    return 0;
}

// ALTERACOES PENDENTES

// Falta adicionar o controle de tempo
// Questao da quantidade de item ainda precisa ser modificada