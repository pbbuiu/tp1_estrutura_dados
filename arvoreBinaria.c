#include "BASE.H"
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

// Funcao responsavel por criar um vetor com os valores do arquivo binario
int* criaVetor(char* arqNome, int* qtdItens, Analise* analisador, bool printer){
    FILE* arquivoBinario = fopen(arqNome, "rb");
    if(arquivoBinario == NULL){
        printf("Erro ao abrir o arquivo binario contendo os numeros\n");
        // return arquivoBinario;
    }

    // Vai ate o final do arquivo e diz quantos valores existem para poder criar um vetor desse tamanho
    fseek(arquivoBinario, 0, SEEK_END); 
    *qtdItens = ftell(arquivoBinario)/sizeof(int); 
    int* vetor = malloc(*qtdItens * sizeof(int));

    // Copia cada elemento do arquivo Binario para o vetor 
    fseek(arquivoBinario, 0, SEEK_SET);
    int numeroAtual;
    int i = 0;
    while (fread(&numeroAtual, sizeof(int), 1, arquivoBinario) == 1) {
        if(printer) printf("Valor %d tranferido do arquivo Binario para o vetor\n", numeroAtual);
        analisador->transferencia++; // +1 transferencia
        vetor[i] = numeroAtual;
        i++;
    }

    fclose(arquivoBinario);

    return vetor;
}

// Funcao responsavel por adicionar um item a arvore
void inserir(FILE* arvore, int novoValor, Analise* analisador){ 

    // Cria o novo item e faz as devidas associacoes
    Item novoItem;
    novoItem.registro.chave = novoValor;
    novoItem.esq = -1;
    novoItem.dir = -1;

    // Insercao do primeiro item, logo eh executado apenas uma vez quando a arvore esta vazia
    if(ftell(arvore) == 0){
        fwrite(&novoItem, sizeof(Item), 1, arvore); // Escreve o primeiro item
        analisador->transferencia++; // +1 transferencia
        return;
    }
        
    fwrite(&novoItem, sizeof(Item), 1, arvore); // Escreve o item no final da arvore
    analisador->transferencia++; // +1 tranferencia 

    int posicaoNovoItem = ftell(arvore)/sizeof(Item) - 1; // Salva a posicao do item que acabou de ser adicionado  
    int posicaoParaAttArvore; // Variavel responsavel por guardar a posicao do ultimo item vizitado pelo ponteiro, importante para atualizacao do esq e dir

    fseek(arvore, 0, SEEK_SET); // Voltamos para o inicio da arvore para fazer a navegacao por ela

    Item ItemAtual; // Variavel que recebe o item lido na arvore, usada para as devidas comparacoes
    while(1){
        posicaoParaAttArvore = ftell(arvore)/sizeof(Item); // Guarda a posicao do ultimo item lido

        fread(&ItemAtual, sizeof(Item), 1, arvore); // Le o item atual
        analisador->transferencia++; // +1 transferencia 

        if(novoItem.registro.chave < ItemAtual.registro.chave){
            analisador->comparacoes++; // +1 comparacao

            // Caso o campo esq seja -1, quer dizer que é um "no" sem filhos, logo é aqui que vamos atualizar o endereco
            if(ItemAtual.esq == -1){ ///////////////////////////////////////////////////////////// nao sei se conta como comparacao para ser adicionada ao analisador
                ItemAtual.esq = posicaoNovoItem; // Variavel na memoria interna recebe a posicao do novo item adicionado 

                fseek(arvore, posicaoParaAttArvore*sizeof(Item), SEEK_SET); ///////////////////////////////////////////////////// explicar melhr aqui
                fwrite(&ItemAtual, sizeof(Item), 1, arvore);
                analisador->transferencia++; // +1 tranferencia

                fseek(arvore, 0, SEEK_END); // Jogamos o ponteiro no final para adicionar o proximo item caso exista

                break;
            }

            // So caimos nesse else caso a item atual possua filho (dir ou esq != -1), logo vamos para a posicao desse filho
            else{
                fseek(arvore, ItemAtual.esq*sizeof(Item), SEEK_SET); // Ponteiro vai do inicio até a posicao do proximo item
            }////////////////////////////////////////////////////////////
        }

        else if(novoItem.registro.chave > ItemAtual.registro.chave){
             analisador->comparacoes++; // +1 comparacao

            // Caso o campo dir seja -1, quer dizer que é um "no" sem filhos, logo é aqui que vamos atualizar o endereco
            if(ItemAtual.dir == -1){ ///////////////////////////////////////////////////////////// nao sei se conta como comparacao para ser adicionada ao analisador
                ItemAtual.dir = posicaoNovoItem; // Variavel na memoria interna recebe a posicao do novo item adicionado 

                fseek(arvore, posicaoParaAttArvore*sizeof(Item), SEEK_SET); ///////////////////////////////////////////////////// explicar melhr aqui
                fwrite(&ItemAtual, sizeof(Item), 1, arvore);
                analisador->transferencia++; // +1 tranferencia

                fseek(arvore, 0, SEEK_END); // Jogamos o ponteiro no final para adicionar o proximo item caso exista

                break;
            }

            // So caimos nesse else caso a item atual possua filho (dir ou esq != -1), logo vamos para a posicao desse filho
            else{
                fseek(arvore, ItemAtual.dir*sizeof(Item), SEEK_SET); // Ponteiro vai do inicio até a posicao do proximo item
            }////////////////////////////////////////////////////////////
        }

        else
            printf("ERROR, nao pode haver chaves iguais\n"); ////////////////////////////////////// pensar no que fazer em caso de itens iguais
    }

}

// Funcao responsavel por navegar pela arvore para encontrar um item
void pesquisar(char* arqNome, int valor, Analise* analisador, bool printer){
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
        if(valor < itemAtual.registro.chave && itemAtual.esq != -1){   
            analisador->comparacoes++; // +1 comparacao 
            if(printer) printf("Comparacao atual: [%d <-> %d] --- Transferencias: %ld Compacoes: %ld\n", itemAtual.registro.chave, valor, analisador->transferencia, analisador->comparacoes);
            fseek(arvore, itemAtual.esq*sizeof(Item), SEEK_SET); // Vai para posicao mostrada pelo esq
        }
        
        // Caso o valor procurado seja maior do o valor atual e diferente de -1, vamos para o item na posicao guardada no dir
        else if(valor > itemAtual.registro.chave && itemAtual.dir != -1){
            analisador->comparacoes++; // +1 comparacao 
            if(printer) printf("Comparacao atual: [%d <-> %d] --- Transferencias: %ld Compacoes: %ld\n", itemAtual.registro.chave, valor, analisador->transferencia, analisador->comparacoes);
            fseek(arvore, itemAtual.dir*sizeof(Item), SEEK_SET); // Vai para a posicao mostrada pelo dir
        }

        // Achamos o item
        else if(valor == itemAtual.registro.chave){
            analisador->comparacoes++; 
            if(printer) printf("Comparacao atual: [%d <-> %d] --- Transferencias: %ld Compacoes: %ld\n", itemAtual.registro.chave, valor, analisador->transferencia, analisador->comparacoes);
            printf("O item %d foi encontrado na posicao %ld da arvore binaria\n", valor, ftell(arvore)/sizeof(Item));
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

int main() {
    char arqNome[30];
    strcpy(arqNome, ""); 
    
    FILE* arvore = fopen("arvoreBinaria.bin", "wb+"); 
    if(arvore == NULL){
        printf("Erro ao criar o arquivo arvoreBinaria.bin\n");
        return 0;
    }


    int printer = false; //////////////// tirar daqio
    
    // Variavel para analise
    Analise analisador;
    inicializaAnalise(&analisador); 


    


    printf("\n======== ARVORE BINARIA MEMORIA EXTERNA ========\n");
    printf("Digite o valor que deseja buscar: ");
    int valor;
    scanf("%d", &valor);

    // Recebe um arquivo binario contendo os valores e transforma em um vetor
    int qtdItens = 0;
    int* vetor;
    vetor = criaVetor("numeros.bin", &qtdItens, &analisador, printer); ////////////////////////////////////// Isso aqui talvez nao precise, preciso entender commo vamos pegar o arquivo binario

    printf("\n- PRE-PROCESSAMENTO -\n\n"); ////////////////////////////////////////////////// falta incluir o tempo

    // Imprime as analises das transferencias para o vetor
    printf("- Criacao do vetor de valores:\n");
    printf("Tempo: %f\nNumero de comparacoes: %ld\nNumero de transferencias: %ld\n\n", analisador.tempoTotal, analisador.comparacoes, analisador.transferencia);
    inicializaAnalise(&analisador); // Reseta os valores do analisador porque ja copiamos os itens para o vetor

    // Insere todos os valores do vetor na arvore binaria
    for(int i = 0; i < qtdItens; i++){
        inserir(arvore, vetor[i], &analisador);
    }

    // Imprime as analises da montagem da arvore
    printf("- Criacao da arvore binaria:\n");
    printf("Tempo: %f\nNumero de comparacoes: %ld\nNumero de transferencias: %ld\n\n", analisador.tempoTotal, analisador.comparacoes, analisador.transferencia);
    inicializaAnalise(&analisador); // Reseta os valores do analisador por que a arvore foi montada 

    // clock_gettime(CLOCK_MONOTONIC, &analisador.inicio);
    pesquisar("arvoreBinaria.bin", valor, &analisador, printer);
    // clock_gettime(CLOCK_MONOTONIC, &analisador.fim);
    // analisador.tempoTotal = (analisador.fim.tv_sec - analisador.inicio.tv_sec) + (analisador.fim.tv_nsec - analisador.inicio.tv_nsec) / 1e9;
    free(vetor);

    // Imprime as analises da pesquisa
    printf("\n- Pesquisa -\n"); /////////////////////////////// falta incluir o tempo
    printf("Tempo: %f\nNumero de comparacoes: %ld\nNumero de transferencias: %ld\n\n", analisador.tempoTotal, analisador.comparacoes, analisador.transferencia);

    // Criacao de um arquivo txt que representa a arvore binaria
    FILE* txt = fopen("rascunho.txt", "w");

    Item conversor;
    fseek(arvore, 0, SEEK_SET);
    for(int i = 0; i < qtdItens; i++){
        fread(&conversor, sizeof(Item), 1, arvore);
        fprintf(txt, "%d %d %d\n", conversor.esq, conversor.registro.chave, conversor.dir);
    }

    fclose(txt);
    fclose(arvore);

    return 0;
}