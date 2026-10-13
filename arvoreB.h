#ifndef ARVOREB_H
#define ARVOREB_H

#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define M 2  //Ordem da minha Árvore B
#define MAX_CHAVES (2 * M) //2m quantidade de itens em cada página
#define NIL (-1L)

#ifdef _WIN32
    #define SEEK64(f, off, o) _fseeki64((f), (long long)(off), (o))
    #define TELL64(f)         _ftelli64(f)
#else
    #define SEEK64(f, off, o) fseeko((f), (off_t)(off), (o))
    #define TELL64(f)         ((long long)ftello(f))
#endif



//removi o "Tipochave"
typedef struct TipoRegistro{
    int chave;//mudei para int, o long de antes da conflito com o registro
    long pos; //adicionei o indice de registro no arquivo de dados
} TipoRegistro;

//typedef struct TipoPagina* TipoApontador; Antes
typedef long TipoApontador; //depois
typedef struct TipoPagina{
    int n;
    TipoRegistro r[MAX_CHAVES];
    TipoApontador p[MAX_CHAVES + 1];
} TipoPagina;

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