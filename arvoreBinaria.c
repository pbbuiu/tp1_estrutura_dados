#include "BASE.H"
#include <stdio.h>
#include <stdlib.h>

typedef struct{
    Registros registro;
    int esq;
    int dir;
} Item;


// Funcao responsavel apenas por adicionar o primeiro item a arvore
void criarArvore(FILE* arvore, int novoValor, Analise* analisador){
    // Cria o item e faz as devidas associacoes
    Item novoItem;
    novoItem.registro.chave = novoValor;
    novoItem.esq = -1;
    novoItem.dir = -1;

    fwrite(&novoItem, sizeof(Item), 1, arvore); // Escreve o primeiro item
    analisador->transferencia++; // +1 transferencia
}

// Funcao responsavel por adicionar um item a arvore
void inserir(FILE* arvore, int novoValor, Analise* analisador){ 
    // Cria o novo item e faz as devidas associacoes
    Item novoItem;
    novoItem.registro.chave = novoValor;
    novoItem.esq = -1;
    novoItem.dir = -1;

    fwrite(&novoItem, sizeof(Item), 1, arvore); // Escreve o item no final da arvore
    analisador->transferencia++; // +1 tranferencia 

    int posicaoNovoItem = ftell(arvore)/sizeof(Item) - 1; // Salva a posicao do item que acabou de ser adicionado  
    int posicaoParaAttArvore; // Variavel responsavel por guardar a posicao do ultimo item vizitado pelo ponteiro, importante para atualizacao do esq e dir

    fseek(arvore, 0, SEEK_SET); // Voltamos para o inicio da arvore para fazer a navegacao por ela

    Item ItemAtual; // Variavel que recebe o item lido na arvore, usada para as devidas comparacoes
    while(1){
        posicaoParaAttArvore = ftell(arvore)/sizeof(Item); ///////////////////////////////////////////////////////////////// explicar melhor isso aqui

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
void pesquisar(int valor, Analise* analisador){

    FILE* arvore = fopen("arvoreBinaria.bin", "rb"); ////////////////////////////////////////////////// Se pa o certo seria passar o arquivo por argumento e não criar ele aq

    Item itemAtual; 
    while(1){
        fread(&itemAtual, sizeof(Item), 1, arvore); // Le o item apontado pelo ponteiro e joga ele na variavel 
        analisador->transferencia++; // +1 transferencia

        // Caso o valor procurado seja menor do o valor atual e diferente de -1, vamos para o item na posicao guardada no esq
        if(valor < itemAtual.registro.chave && itemAtual.esq != -1){   
            analisador->comparacoes++; // +1 comparacao
            fseek(arvore, itemAtual.esq*sizeof(Item), SEEK_SET); // Vai para posicao mostrada pelo esq
            printf("%d\n", itemAtual.registro.chave); ///////////////////////////////// Retirar isso aqui depois
        }
        
        // Caso o valor procurado seja maior do o valor atual e diferente de -1, vamos para o item na posicao guardada no dir
        else if(valor > itemAtual.registro.chave && itemAtual.dir != -1){
            analisador->comparacoes++; // +1 comparacao 
            fseek(arvore, itemAtual.dir*sizeof(Item), SEEK_SET); // Vai para a posicao mostrada pelo dir
            printf("%d\n", itemAtual.registro.chave); ////////////////////////////// tirar isso depois

        }

        // Achamos o item
        else if(valor == itemAtual.registro.chave){
            analisador->comparacoes++; ////////////////////////////// talvez nao seja considera uma comparacao
            printf("Achou na posicao %ld\n", ftell(arvore)/sizeof(Item));
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
    FILE* arvore = fopen("arvoreBinaria.bin", "wb+"); 
    Analise analisador = inicializaAnalise(); 

    criarArvore(arvore, 50, &analisador); // Não sei se é o ideal fazer algo assim




// Toda essa parte e provisoria, responsavel por criar um arquivo txt que represente o arquivo binario da arvore
    FILE* txt = fopen("rascunho.txt", "w");
    int qtdNumeros = 1000;
    int vetor[qtdNumeros];
    int qtd = 0;
    int repetido;

    while (qtd < qtdNumeros) {
        int novoValor = rand() % 10000;
        if(novoValor == 50) break;
        repetido = 0;

        for (int i = 0; i < qtd; i++) {
            if (vetor[i] == novoValor) {
                repetido = 1;
                break;
            }
        }

        if (repetido == 0) {
            vetor[qtd] = novoValor;
            inserir(arvore, novoValor, &analisador);
            qtd++;
        }
    }

    Item conversor;
    fseek(arvore, 0, SEEK_SET);
    for(int i = 0; i < qtdNumeros; i++){
        fread(&conversor, sizeof(Item), 1, arvore);
        fprintf(txt, "%d %d %d\n", conversor.esq, conversor.registro.chave, conversor.dir);
    }







    fclose(arvore);
    // fclose(txt);

    printf("Arvore montada, qual valor voce quer procurar nela?\n");
    int valor;
    scanf("%d", &valor);
    pesquisar(valor, &analisador);

    printf("\nAnalises:\n");
    printf("Numero de transferencias: %ld\nNumero de comparacoes: %ld", analisador.transferencia, analisador.comparacoes);

    return 0;
}

