#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ITENSPAGINA 100

typedef struct{
    Registros registro;
    int esq;
    int dir;
} Item;

void inserir(FILE* arvore, Registros *novoValor, Analise* analisador){ 
    // Cria o novo item com o resgistro
    Item novoItem;
    novoItem.registro = *novoValor;
    novoItem.esq = -1;
    novoItem.dir = -1;

    fwrite(&novoItem, sizeof(Item), 1, arvore); 
    analisador->transferencia++;

    int posicaoNovoItem = ftell(arvore)/sizeof(Item) - 1; //Armazena a posição do item adicionado
    fseek(arvore, 0, SEEK_SET);

    Item ItemAtual; 

    //Se inicia a pesquisa para encontrar quem será o pai do item adicionado
    while(fread(&ItemAtual, sizeof(Item), 1, arvore)){
        analisador->transferencia++;

        if(novoItem.registro.chave < ItemAtual.registro.chave){
            analisador->comparacoes++;

            //Esquerda sem filhos
            if(ItemAtual.esq == -1){ 
                analisador->comparacoes++;
                ItemAtual.esq = posicaoNovoItem;

                fseek(arvore, -1*(long)sizeof(Item), SEEK_CUR);//Retorna para o item que acabou de ser lido
                fwrite(&ItemAtual, sizeof(Item), 1, arvore);
                analisador->transferencia++; 

                fseek(arvore, 0, SEEK_END);//Retorna para o final do arquivo
                break;
            }
            //Achou filho na esquerda
            else{
                fseek(arvore, ItemAtual.esq*sizeof(Item), SEEK_SET); //Vai para a posição do filho à esquerda
                continue;
            }
            
        }
        //Caso seja
        else if(novoItem.registro.chave > ItemAtual.registro.chave){ 
                if (ItemAtual.dir == -1){
                    analisador->comparacoes++;
                    ItemAtual.dir = posicaoNovoItem; // Variavel na memoria interna recebe a posicao do novo item adicionado 

                    fseek(arvore, -1*(long)sizeof(Item), SEEK_SET);//Retorna para o item que acabou de ser lido
                    fwrite(&ItemAtual, sizeof(Item), 1, arvore);//Reescreve o item com o novo filho
                    analisador->transferencia++;

                    fseek(arvore, 0, SEEK_END);//Retorna para o final do arquivo
                    break;
                }
            else{
                fseek(arvore, ItemAtual.dir*sizeof(Item), SEEK_SET); //Vai para a posição do filho à direita
                continue;
            }
        }
        //Caso seja repetida ou seja a primeira
        break;
    }

}

long int pesquisar(FILE *arvore, Registros *itemProcurado, Analise* analisador){
    Item itemAtual;
    fseek(arvore, 0, SEEK_SET);
    while(fread(&itemAtual, sizeof(Item), 1, arvore)){
        analisador->transferencia++;
        
        //Chave procurada menor que a chave do item atual
        analisador->comparacoes++;
        if(itemProcurado->chave < itemAtual.registro.chave){   
            analisador->comparacoes++;
            if (itemAtual.esq == -1){ //Significa que não existe filho a esquerda
                printf("A chave %d nao existe\n", itemProcurado->chave);
                return 0;
            }
            fseek(arvore, itemAtual.esq*sizeof(Item), SEEK_SET); //Vai para a posição do filho a esquerda
            continue;
        }

        //Chave procurada maior que a chave do item atual
        analisador->comparacoes++;
        if(itemProcurado->chave > itemAtual.registro.chave){
            analisador->comparacoes++; 
            if (itemAtual.dir == -1){
                printf("A chave %d nao existe\n", itemProcurado->chave);
                return 0;
            }
            fseek(arvore, itemAtual.dir*sizeof(Item), SEEK_SET); //Vai para a posição do filho a direita
            continue;
        }

        //Só resta ser igual
        *itemProcurado = itemAtual.registro;
        return (long int)ftell(arvore)/sizeof(Item) - 1;
    }
}   

bool realizaPesquisaBinaria (Analise *a, Registros *itemPesquisado, long int qtdRegistrosArq, char *situacao, bool p){
    FILE* arvore = fopen("arvoreBinaria.bin", "wb+");
    FILE* arq = fopen(situacao, "rb");
    int resto = qtdRegistrosArq % ITENSPAGINA;
    int cont = 0;
    Registros pagina[ITENSPAGINA];

    while(((cont+1)*ITENSPAGINA < qtdRegistrosArq) && fread(pagina, sizeof(Registros), ITENSPAGINA, arq) == ITENSPAGINA){
        for (int i=0; i<ITENSPAGINA; i++){
            inserir(arvore, &pagina[i], a);

        }
    }
    a->comparacoes++;
    if (resto != 0){
        for (int i=0; i<resto; i++)
            inserir(arvore, &pagina[i], a);
    }
    else
        a->transferencia++;
    
    long int posItemPesquisado = pesquisar(arvore, itemPesquisado, a);
    if (posItemPesquisado){
        printf("Item encontrado na posição %ld\n", posItemPesquisado);
        return true;
    }
    else{
        printf("Item não encontrado\n");
    }
}

