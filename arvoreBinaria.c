#include "arvoreBinaria.h"
#include <stdio.h>








/*
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define M 2  //Ordem da minha Árvore B
#define MM 4 //2m quantidade de itens em cada página

typedef long TipoChave;

typedef struct TipoRegistro{
    TipoChave chave;
} TipoRegistro;

typedef struct TipoPagina* TipoApontador;

typedef struct TipoPagina{
    int n;
    TipoRegistro r[MM];
    TipoApontador p[MM + 1];
} TipoPagina;

void inicializador(TipoApontador *arvore){
    *arvore = NULL;
}

bool pesquisa(TipoApontador ap, TipoRegistro *x){
    int i = 1;

    if (!ap){
        printf("Nao foi possivel encontrar o item\n");
        return;
    }

    //Agora vamos procurar o ramo em que devemos descer na árvore 
    while(i < ap->n && x->chave > ap->r[i-1].chave) i++;

    if (x->chave == ap->r[i-1].chave){
        *x = ap->r[i-1];
        return true;
    }

    if (i < ap->n)  return pesquisa(ap->p[i-1], x); 
    else            return pesquisa(ap->p[i], x);
}

bool ins(TipoRegistro x, TipoApontador ap, bool *cresceu, TipoRegistro *regRetorno, TipoApontador *apRetorno);

void insereNaPagina(TipoApontador ap, TipoRegistro x, TipoApontador apDir);

bool insere(TipoApontador *ap, TipoRegistro x){
    if (!ap) return false;

    bool cresceu;
    TipoRegistro regRetorno;
    TipoApontador *apRetorno, *apTemp;

    ins(x, *ap, &cresceu, &regRetorno, &apRetorno);
}

bool ins(TipoRegistro x, TipoApontador ap, bool *cresceu, TipoRegistro *regRetorno, TipoApontador *apRetorno){
    int i = 1;
    int j;
    TipoApontador apTemp;

    //Caso base: Passou do nível folha, ou seja, chegou aonde precisa ser inserido
    if (ap == NULL){
        *cresceu = true;
        *regRetorno = x;
        *apRetorno = NULL;
        return;
    }
    //Varredura para continuar descendo entre os ramos 
    while(i < ap->n && x.chave > ap->r[i-1].chave) i++;

    //Se a chave for igual significa que a chave já existe e não é possível adicioná-la novamente
    if (x.chave == ap->r[i-1].chave){
        *cresceu = false;
        return false;
    }

    //Caso chegue no último e a chave ainda seja maior não irá subtrair 1 no i para descer no último ramo
    if (x.chave < ap->r[i-1].chave) i--;

    //Desse recursivamente fazendo a pesquisa até encontrar a página que o elemento deve ser inserido
    return ins(x, ap->p[i], cresceu, regRetorno, apRetorno);

    //Se a recursividade retornar com o cresceu == false significa que deu tudo certo
    if (!(*cresceu)) return true;

    if (ap->n < MM){
        insereNaPagina(ap, *regRetorno, *apRetorno);
        *cresceu = false;
        return true;
    }
    
    apTemp = (TipoApontador) malloc (sizeof(TipoPagina));
    apTemp->n = 0;
    apTemp->p[0] = NULL;

    //Se o novo item estiver na metade esquerda ele fica na página onde deveria, senão ele vai pra nova página temporária de uma vez
    if (i < M + 1){
        insereNaPagina(apTemp, ap->r[MM - 1], ap->p[MM]);
        ap->n--;
        insereNaPagina(ap, *regRetorno, *apRetorno);
    }
    else{
        insereNaPagina(ap, *regRetorno, *apRetorno);
    }

    //Insiro os outros itens da direita que faltaram na nova página
    for (j = M + 1; j < MM; j++){
        insereNaPagina(apTemp, ap->r[j], ap->p[j+1]);
    }

    //Ajusta a página existente para o tamanho mínimo, pois M itens já terão sido passados para a nova página
    ap->n = M;

    //Passaria os filhos do elemento do meio para o seu novo filho em apTemp
    apTemp->p[0] = ap->p[M+1];

    //Retornando para a recursividade anterior o novo elemento que deve ser inserido na página pai
    *regRetorno = ap->r[M];

    //Retornando também os filhos desse novo elemento que foi para página pai
    *apRetorno = apTemp;
}

void insereNaPagina(TipoApontador ap, TipoRegistro x, TipoApontador apDir){
    int k = ap->n;

    //Vasculho as chaves até encontrar onde o registro se encaixa k-1 < x < k
    while(k >= 1 && ap->r[k-1].chave > x.chave ){
        ap->r[k] = ap->r[k-1];
        ap->p[k+1] = ap->p[k];
        k--;
    }

    ap->r[k] = x;
    ap->p[k+1] = apDir;
    ap->n++;
}
*/