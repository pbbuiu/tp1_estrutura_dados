<<<<<<< HEAD
#ifndef ARVOREB_H
#define ARVOREB_H

#include "base.h"
=======
#include "arvoreBinaria.h"
>>>>>>> ffcbf29fcd97ed235c1969b9dc484187132c83cb
#include <stdio.h>








/*
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gerador.h"

#define M 2  //Ordem da minha Árvore B
#define MM 4 //2m quantidade de itens em cada página

typedef long TipoChave;

<<<<<<< HEAD


//removi o "Tipochave"
=======
>>>>>>> ffcbf29fcd97ed235c1969b9dc484187132c83cb
typedef struct TipoRegistro{
    TipoChave chave;
} TipoRegistro;

<<<<<<< HEAD
//typedef struct TipoPagina* TipoApontador; Antes
typedef long TipoApontador; //depois
=======
typedef struct TipoPagina* TipoApontador;

>>>>>>> ffcbf29fcd97ed235c1969b9dc484187132c83cb
typedef struct TipoPagina{
    int n;
    TipoRegistro r[MM];
    TipoApontador p[MM + 1];
} TipoPagina;

<<<<<<< HEAD
typedef struct{
    FILE *arqArvore; //arvoreB.bin (páginas)
    FILE *arqDados; //arq_*.bin 
    TipoApontador raiz;
    long numPaginas;
}ArvoreB;

void lePaginaB(ArvoreB *t, TipoApontador pos, TipoPagina *pag, Analise *a){
       SEEK64(t->arqArvore, (long long)pos * (long long)sizeof(TipoPagina), SEEK_SET);
    if ( fread(pag, sizeof(TipoPagina), 1 , t->arqArvore) != 1){
        printf("Erro ao ler a páhina %ld da arvoreB\n", pos);
        exit(1);
    }
    a->transferencia++;
}

void escrevePaginaB(ArvoreB *t, TipoApontador pos, TipoPagina *pag){
    SEEK64(t->arqArvore, (long long)pos * (long long)sizeof(TipoPagina), SEEK_SET);
    if (fwrite(pag, sizeof(TipoPagina), 1, t->arqArvore) != 1){
        printf("Erro ao escrever a pagina %ld da arvore B\n", pos);
        exit(1);
    }
}

void inicializaPaginaB(TipoPagina *pag){
    pag->n = 0;
    for (int i = 0; i <= MAX_CHAVES; i++) pag->p[i] = NIL;
}


void insereNaPagina(TipoPagina *pag, TipoRegistro x, TipoApontador apDir, Analise *a){
    int k = pag->n;
 
    //Vasculha as chaves ate encontrar onde o registro se encaixa
    while (k >= 1){
        a->comparacoes++;
        if (pag->r[k-1].chave > x.chave){
            pag->r[k]   = pag->r[k-1];
            pag->p[k+1] = pag->p[k];
            k--;
        }
        else break;
    }
 
    pag->r[k]   = x;
    pag->p[k+1] = apDir;
    pag->n++;
}

bool ins(ArvoreB *t, TipoRegistro x, TipoApontador ap, bool *cresceu,
         TipoRegistro *regRet, TipoApontador *apRet, Analise *a){
 
    //Caso base: passou da folha, e aqui que o item deve ser inserido
    if (ap == NIL){
        *cresceu = true;
        *regRet = x;
        *apRet = NIL;
        return true;
    }
 
    TipoPagina pag;
    lePaginaB(t, ap, &pag, a);
 
    //Varredura para continuar descendo entre os ramos
    int i = 1;
    while (i < pag.n){
        a->comparacoes++;
        if (x.chave > pag.r[i-1].chave) i++;
        else break;
    }
 
    //Chave igual: ja existe, nao pode ser inserida novamente
    a->comparacoes++;
    if (x.chave == pag.r[i-1].chave){
        *cresceu = false;
        return false;
    }
 
    //Se a chave for menor, desce pelo ramo da esquerda
    a->comparacoes++;
    if (x.chave < pag.r[i-1].chave) i--;
 
    //Desce recursivamente ate a pagina onde o elemento deve ser inserido
    bool ok = ins(t, x, pag.p[i], cresceu, regRet, apRet, a);
    if (!ok || !(*cresceu)) return ok; //duplicata ou nada a propagar
 
    //A pagina tem espaco: insere e grava no disco
    if (pag.n < MAX_CHAVES){
        insereNaPagina(&pag, *regRet, *apRet, a);
        escrevePaginaB(t, ap, &pag);
        *cresceu = false;
        return true;
    }
 
    //Overflow: rachadura da pagina
    TipoPagina temp;
    inicializaPaginaB(&temp);
 
    //Se o novo item cai na metade esquerda ele fica na pagina atual, senao vai para a nova
    if (i < M + 1){
        insereNaPagina(&temp, pag.r[MAX_CHAVES - 1], pag.p[MAX_CHAVES], a);
        pag.n--;
        insereNaPagina(&pag, *regRet, *apRet, a);
    }
    else{
        insereNaPagina(&temp, *regRet, *apRet, a);
    }
 
    //Passa para a nova pagina os itens da direita que faltaram
    for (int j = M + 1; j < MAX_CHAVES; j++){
        insereNaPagina(&temp, pag.r[j], pag.p[j+1], a);
    }
 
    pag.n = M;                    //pagina atual fica com M itens
    temp.p[0] = pag.p[M+1];       //filho a esquerda do primeiro item da nova pagina
    *regRet = pag.r[M];           //item do meio sobe para o pai
 
    TipoApontador novo = t->numPaginas++;
    *apRet = novo;                //e a nova pagina e seu filho a direita
 
    escrevePaginaB(t, ap,   &pag);
    escrevePaginaB(t, novo, &temp);
 
    *cresceu = true;
    return true;
}


bool insereB(ArvoreB *t, TipoRegistro x, Analise *a){
    bool cresceu;
    TipoRegistro regRet;
    TipoApontador apRet;
 
    if (!ins(t, x, t->raiz, &cresceu, &regRet, &apRet, a)) return false;
 
    //A raiz rachou (ou a arvore estava vazia): cria uma nova raiz, a arvore cresce em altura
    if (cresceu){
        TipoPagina nova;
        inicializaPaginaB(&nova);
        nova.n = 1;
        nova.r[0] = regRet;
        nova.p[0] = t->raiz;
        nova.p[1] = apRet;
 
        TipoApontador pos = t->numPaginas++;
        escrevePaginaB(t, pos, &nova);
        t->raiz = pos;
    }
    return true;
}

bool pesquisaB(ArvoreB *t, TipoApontador ap, TipoRegistro *x, Analise *a){
    if (ap == NIL) return false;
 
    TipoPagina pag;
    lePaginaB(t, ap, &pag, a);
 
    //Procura o ramo em que devemos descer
    int i = 1;
    while (i < pag.n){
        a->comparacoes++;
        if (x->chave > pag.r[i-1].chave) i++;
        else break;
    }
 
    a->comparacoes++;
    if (x->chave == pag.r[i-1].chave){
        *x = pag.r[i-1];
        return true;
    }
 
    a->comparacoes++;
    if (x->chave < pag.r[i-1].chave) return pesquisaB(t, pag.p[i-1], x, a);
    else                             return pesquisaB(t, pag.p[i],   x, a);
}

static void imprimeChavesArquivo(FILE *arq, long qtd){
    Registros reg;
    SEEK64(arq, 0, SEEK_SET);
    printf("\n= Chaves dos registros do arquivo =\n");
    for (long i = 0; i < qtd && fread(&reg, sizeof(Registros), 1, arq) == 1; i++)
        printf("%d ", reg.chave);
    printf("\n\n");
    SEEK64(arq, 0, SEEK_SET);
}
 
static void calculaTempo(Analise *a){
    clock_gettime(CLOCK_MONOTONIC, &a->fim);
    a->tempoTotal = (a->fim.tv_sec - a->inicio.tv_sec) + (a->fim.tv_nsec - a->inicio.tv_nsec) / 1e9;
}

bool realizaPesquisaArvoreB(Analise *a, Registros *itemPesquisado, long int qtdRegistrosArq, int situacao, bool p){
    char *nomes[3] = {"arq_crescente.bin", "arq_decres.bin", "arq_aleatorio.bin"};
 
    if (situacao < 1 || situacao > 3){
        printf("Situacao invalida: use 1 (crescente), 2 (decrescente) ou 3 (aleatorio)\n");
        return false;
    }
    if (qtdRegistrosArq <= 0){
        printf("A quantidade de registros deve ser positiva\n");
        return false;
    }
    char *nomeDados = nomes[situacao - 1];
 
    //Abre o arquivo de dados; se nao existir (ou tiver outra quantidade) ele e criado
    FILE *arqDados = fopen(nomeDados, "rb");
    long long qtdNoArquivo = -1;
    if (arqDados){
        SEEK64(arqDados, 0, SEEK_END);
        qtdNoArquivo = TELL64(arqDados) / (long long)sizeof(Registros);
        SEEK64(arqDados, 0, SEEK_SET);
    }
    if (!arqDados || qtdNoArquivo != qtdRegistrosArq){
        if (arqDados) fclose(arqDados);
        printf("Gerando o arquivo %s com %ld registros...\n", nomeDados, qtdRegistrosArq);
        if (!geraArquivo(nomeDados, (int)qtdRegistrosArq, situacao)) return false;
        arqDados = fopen(nomeDados, "rb");
        if (!arqDados){
            printf("Erro ao abrir o arquivo %s\n", nomeDados);
            return false;
        }
    }
    ArvoreB t;
    t.arqDados = arqDados;
    t.raiz = NIL;
    t.numPaginas = 0;
    t.arqArvore = fopen("arvoreB.bin", "wb+");
    if (!t.arqArvore){
        printf("Erro ao criar o arquivo arvoreB.bin\n");
        fclose(arqDados);
        return false;
    }
 
    if (p) imprimeChavesArquivo(arqDados, qtdRegistrosArq);
 
    printf("========ARVORE B (M = %d)========\n\n", M);
    inicializaAnalise(a);
    clock_gettime(CLOCK_MONOTONIC, &a->inicio);
 
    Registros reg;
    for (long i = 0; i < qtdRegistrosArq; i++){
        if (fread(&reg, sizeof(Registros), 1, arqDados) != 1){
            printf("Erro ao ler o registro %ld do arquivo de dados\n", i);
            fclose(arqDados); fclose(t.arqArvore);
            return false;
        }
        a->transferencia++; //registro lido da memoria externa
 
        TipoRegistro novo;
        novo.chave = reg.chave;
        novo.pos = i;
        if (!insereB(&t, novo, a))
            printf("Aviso: chave %d repetida, ignorada\n", reg.chave);
    }
    calculaTempo(a);
 
    printf("- Pre-processamento -\n");
    printf("Tempo: %.9lf\n", a->tempoTotal);
    printf("Numero de comparacoes: %ld\n", a->comparacoes);
    printf("Numero de transferencias: %ld\n\n", a->transferencia);
    inicializaAnalise(a);
    clock_gettime(CLOCK_MONOTONIC, &a->inicio);
 
    TipoRegistro x;
    x.chave = itemPesquisado->chave;
    x.pos = -1;
    bool achou = pesquisaB(&t, t.raiz, &x, a);
 
    if (achou){
        //Le o registro completo no arquivo de dados
        SEEK64(arqDados, (long long)x.pos * (long long)sizeof(Registros), SEEK_SET);
        if (fread(itemPesquisado, sizeof(Registros), 1, arqDados) != 1){
            printf("Erro ao ler o registro completo\n");
            fclose(arqDados); fclose(t.arqArvore);
            return false;
        }
        a->transferencia++;
    }
    calculaTempo(a);
 
    printf("- Pesquisa -\n");
    printf("Tempo: %.9lf\n", a->tempoTotal);
    printf("Numero de comparacoes: %ld\n", a->comparacoes);
    printf("Numero de transferencias: %ld\n\n", a->transferencia);
 
    if (achou){
        printf("Registro encontrado na posicao %ld do arquivo:\n", x.pos + 1);
        printf("chave = %d\ndado1 = %ld\ndado2 = %s\n", itemPesquisado->chave, itemPesquisado->dado1, itemPesquisado->dado2);
    }
    else printf("A chave %d nao foi encontrada\n", itemPesquisado->chave);
 
    fclose(arqDados);
    fclose(t.arqArvore);
    return achou;
}

#endif // ARVOREB_H
=======
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
>>>>>>> ffcbf29fcd97ed235c1969b9dc484187132c83cb
