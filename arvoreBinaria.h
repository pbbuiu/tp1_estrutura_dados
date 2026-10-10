#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int chave;
    int esq;
    int dir;
} Item;

void criarArvore(FILE* arvore, int novoValor){
    Item novoItem;
    novoItem.chave = novoValor;
    novoItem.esq = -1;
    novoItem.dir = -1;

    fwrite(&novoItem, sizeof(Item), 1, arvore);
}

void inserir(FILE* arvore, int novoValor){
    Item novoItem;
    novoItem.chave = novoValor;
    novoItem.esq = -1;
    novoItem.dir = -1;

    fwrite(&novoItem, sizeof(Item), 1, arvore);

    int posicaoNovoItem = ftell(arvore)/sizeof(Item) - 1; 
    int posicaoParaAttArvore;

    fseek(arvore, 0, SEEK_SET);

    Item ItemAtual;
    while(1){
        posicaoParaAttArvore = ftell(arvore)/sizeof(Item);

        fread(&ItemAtual, sizeof(Item), 1, arvore);
        if(novoItem.chave > ItemAtual.chave)
            if(ItemAtual.dir == -1){
                ItemAtual.dir = posicaoNovoItem; 

                fseek(arvore, posicaoParaAttArvore*sizeof(Item), SEEK_SET); // Para atualizar no arquivo binario
                fwrite(&ItemAtual, sizeof(Item), 1, arvore);

                fseek(arvore, 0, SEEK_END);  

                break;
            }

            else{
                fseek(arvore, ItemAtual.dir*sizeof(Item), SEEK_SET); // Esta indo pro inicio todas as vezes, o ideal seria pular direto para posicao seguinte
            }

        else if(novoItem.chave < ItemAtual.chave)
             if(ItemAtual.esq == -1){
                ItemAtual.esq = posicaoNovoItem; 

                fseek(arvore, posicaoParaAttArvore*sizeof(Item), SEEK_SET); 
                fwrite(&ItemAtual, sizeof(Item), 1, arvore);

                fseek(arvore, 0, SEEK_END);  

                break;
            }

            else{
                fseek(arvore, ItemAtual.esq*sizeof(Item), SEEK_SET); // Esta indo pro inicio todas as vezes, o ideal seria pular direto para posicao seguinte
            }

        else
            printf("ERROR, nao pode haver chaves iguais\n");
    }

}

void pesquisar(int valor){

    FILE* arvore = fopen("abb.bin", "rb"); // Se pa o certo seria passar o arquivo por argumento e não criar ele aq

    Item itemAtual;
    while(1){
        fread(&itemAtual, sizeof(Item), 1, arvore);
        if(valor < itemAtual.chave && itemAtual.esq != -1){
            fseek(arvore, itemAtual.esq*sizeof(Item), SEEK_SET);
            printf("%d\n", itemAtual.chave);
        }
        
        else if(valor > itemAtual.chave && itemAtual.dir != -1){
            fseek(arvore, itemAtual.dir*sizeof(Item), SEEK_SET);
            printf("%d\n", itemAtual.chave);

        }

        else if(valor == itemAtual.chave){
            printf("Achou na posicao %ld\n", ftell(arvore)/sizeof(Item)-1);
            break;
        }

        else{
            printf("Item nao existe na arvore\n");
            break;
        }
    }

    fclose(arvore);
}   

int main() {
    FILE* arvore = fopen("abb.bin", "wb+"); 
    criarArvore(arvore, 50); // Não sei se é o ideal fazer algo assim

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
            inserir(arvore, novoValor);
            qtd++;
        }
    }


    Item conversor;
    fseek(arvore, 0, SEEK_SET);
    for(int i = 0; i < qtdNumeros; i++){
        fread(&conversor, sizeof(Item), 1, arvore);
        fprintf(txt, "%d %d %d\n", conversor.esq, conversor.chave, conversor.dir);
    }


    fclose(arvore);
    fclose(txt);

    printf("Arvore montada, qual valor voce quer procurar nela?\n");
    int valor;
    scanf("%d", &valor);
    pesquisar(valor);

    return 0;
}

