#ifndef ARVOREB_H
#define ARVOREB_H

#include "base.h"
#include "geraBin.h"
#include "arvoreBinaria.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define M 2  //Ordem da minha Árvore B
#define MAX_CHAVES (2 * M) //2m quantidade de itens em cada página
#define NIL (-1L)

//removi o "Tipochave"
typedef struct TipoRegistro{
    int chave;//mudei para int, o long de antes da conflito com o registro
    long pos; //adicionei o indice de registro no arquivo de dados
} TipoRegistro;

//typedef struct TipoPagina* TipoApontador; Antes
typedef long TipoApontador //depois
typedef struct TipoPagina{
    int n;
    TipoRegistro r[MAX_CHAVES];
    TipoApontador p[MAX_CHAVES + 1];
} TipoPagina;

typedef struct{
    FILE *arqArvore; //arvoreB.bin (páginas)
    FILE *arqDados; //arq_*.bin 
    TipoApontador raiz;
    long numPaginas
}ArvoreB;

void inicializador(TipoApontador *arvore){
    *arvore = NULL;
}

bool pesquisa(TipoApontador ap, TipoRegistro *x){
    int i = 1;

    if (!ap){
        printf("Nao foi possivel encontrar o item\n");
        return false;
    }

    //Agora vamos procurar o ramo em que devemos descer na árvore 
    while(i < ap->n && x->chave > ap->r[i-1].chave) i++;

    if (x->chave == pag.r[i-1].chave) { *x = pag.r[i-1]; return true; }
    if (x->chave <  pag.r[i-1].chave) return pesquisaB(t, pag.p[i-1], x, a);
    else                              return pesquisaB(t, pag.p[i],   x, a);

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

    if (cresceu) { //caso a raiz rache
    TipoPagina nova; inicializaPaginaB(&nova);
    nova.n = 1;  nova.r[0] = regRet;
    nova.p[0] = t->raiz;  nova.p[1] = apRet;
    TipoApontador pos = t->numPaginas++;
    escrevePaginaB(t, pos, &nova);
    t->raiz = pos;
}
    //Caso base: Passou do nível folha, ou seja, chegou aonde precisa ser inserido
    if (ap == NULL){
        *cresceu = true;
        *regRetorno = x;
        *apRetorno = NULL;
        return true;
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
    bool ok = ins(t, x, pag.p[i], cresceu, regRet, apRet, a);
    if (!ok || !*cresceu) return ok;   // duplicata ou nada a propagar

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

void lePaginaB(ArvoreB *t, TipoApontador pos, TipoPagina *pag, Analise *a){
    fseek(t->arqArvore, pos * sizeof(TipoPagina), SEEK_SET);
    if ( fread(pag, sizeof(TipoPagina), 1 , t->arqArvore) != 1){
        printf("Erro ao ler a páhina %ld da arvoreB", pos);
        exit(1);
    }
    a->transferencia++;
}

void escrevePaginaB(){


}

bool realizaPesquisaArvoreB(Analise *a, Registros *itemPesquisado, long int qtdRegistrosArq, int situacao, bool p){
    clock_gettime(CLOCK_MONOTONIC, &a->inicio); //Inicio do contador de tempo de pre-processamento
    int cont = 0;
   char *nomes[3] = {"arq_crescente.bin", "arq_decres.bin", "arq_aleatorio.bin"};
    if (situacao < 1 || situacao > 3) return false;
     char *nomeDados = nomes[situacao - 1];


}

#endif // ARVOREB_H