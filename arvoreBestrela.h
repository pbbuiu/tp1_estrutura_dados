#ifndef arvoreBestrela_h
#define arvoreBestrela_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "base.h"

#define M 2
#define MM 4
#define M2 2
#define MM2 4

typedef struct TipoPaginaEstrela* TipoApontadorEstrela;

typedef enum {Interna, Externa} TipoIntExt;

typedef struct TipoPaginaEstrela {
    TipoIntExt Pt;
    union
    {
        struct
        {
            int ni;
            int ri[MM];
            TipoApontadorEstrela pi[MM + 1];
        } U0;

        struct
        {
            int ne;
            Registros re[MM2];
            TipoApontadorEstrela prox;
            TipoApontadorEstrela ante;
        } U1;
    } UU;
} TipoPaginaEstrela;

static void CalculaTempoArvoreBEstrela(Analise *a)
{
    clock_gettime(CLOCK_MONOTONIC, &a->fim);
    a->tempoTotal = (a->fim.tv_sec - a->inicio.tv_sec) + (a->fim.tv_nsec - a->inicio.tv_nsec) / 1e9;
}

void InicializadorEstrela(TipoApontadorEstrela *arvore)
{
    *arvore = NULL;
}

bool PesquisaEstrela(int Ch, Registros *x, TipoApontadorEstrela *Ap, Analise *a)
{
    int i;
    TipoApontadorEstrela Pag = *Ap;

    if (Pag == NULL)
        return false;

    if (Pag->Pt == Interna)
    {
        i = 0;

        while (i < Pag->UU.U0.ni)
        {
            a->comparacoes++;
            if (Ch >= Pag->UU.U0.ri[i])
                i++;
            else
                break;
        }

        return PesquisaEstrela(Ch, x, &Pag->UU.U0.pi[i], a);
    }

    i = 0;

    while (i < Pag->UU.U1.ne)
    {
        a->comparacoes++;
        if (Ch > Pag->UU.U1.re[i].chave)
            i++;
        else
            break;
    }

    if (i < Pag->UU.U1.ne)
    {
        a->comparacoes++;
        if (Ch == Pag->UU.U1.re[i].chave)
        {
            *x = Pag->UU.U1.re[i];
            return true;
        }
    }
    return false;
}

void InsereNaPaginaEstrela(TipoApontadorEstrela Ap, Registros Reg, TipoApontadorEstrela ApDir, Analise *a)
{
    int k;

    if (Ap->Pt == Interna)
    {
        k = Ap->UU.U0.ni;

        while (k > 0)
        {
            a->comparacoes++;
            if (Reg.chave < Ap->UU.U0.ri[k - 1])
            {
                Ap->UU.U0.ri[k] = Ap->UU.U0.ri[k - 1];
                Ap->UU.U0.pi[k + 1] = Ap->UU.U0.pi[k];
                k--;
            }
            else
                break;
        }

        Ap->UU.U0.ri[k] = Reg.chave;
        Ap->UU.U0.pi[k + 1] = ApDir;
        Ap->UU.U0.ni++;
    }
    else
    {
        k = Ap->UU.U1.ne;

        while (k > 0)
        {
            a->comparacoes++;

            if (Reg.chave < Ap->UU.U1.re[k - 1].chave)
            {
                Ap->UU.U1.re[k] = Ap->UU.U1.re[k - 1];
                k--;
            }
            else
                break;
        }

        Ap->UU.U1.re[k] = Reg;
        Ap->UU.U1.ne++;
    }
}

void InsEstrela(Registros Reg, TipoApontadorEstrela Ap, bool *Cresceu, Registros *RegRetorno, TipoApontadorEstrela *ApRetorno, Analise *a)
{
    int i = 0, j;
    TipoApontadorEstrela ApTemp;

    *Cresceu = false;

    if (Ap->Pt == Interna)
    {
        while (i < Ap->UU.U0.ni)
        {
            a->comparacoes++;
            if (Reg.chave >= Ap->UU.U0.ri[i])
                i++;
            else
                break;
        }

        InsEstrela(Reg, Ap->UU.U0.pi[i], Cresceu, RegRetorno, ApRetorno, a);

        if (!*Cresceu)
            return;

        if (Ap->UU.U0.ni < MM)
        {
            InsereNaPaginaEstrela(Ap, *RegRetorno, *ApRetorno, a);
            *Cresceu = false;
            return;
        }

        //Overflow: página interna precisa ser dividida.
        int chaves[MM + 1];
        TipoApontadorEstrela filhos[MM + 2];

        for (j = 0; j < i; j++)
            chaves[j] = Ap->UU.U0.ri[j];

        chaves[i] = RegRetorno->chave;

        for (j = i + 1; j < MM + 1; j++)
            chaves[j] = Ap->UU.U0.ri[j - 1];

        for (j = 0; j <= i; j++)
            filhos[j] = Ap->UU.U0.pi[j];

        filhos[i + 1] = *ApRetorno;

        for (j = i + 2; j < MM + 2; j++)
            filhos[j] = Ap->UU.U0.pi[j - 1];

        ApTemp = malloc(sizeof(TipoPaginaEstrela));

        if (ApTemp == NULL)
        {
            printf("Erro ao alocar pagina interna.\n");
            *Cresceu = false;
            return;
        }

        ApTemp->Pt = Interna;
        RegRetorno->chave = chaves[M];

        Ap->UU.U0.ni = M;
        ApTemp->UU.U0.ni = M;

        for (i = 0, j = M + 1; i < M; i++, j++)
        {
            Ap->UU.U0.ri[i] = chaves[i];
            Ap->UU.U0.pi[i] = filhos[i];

            ApTemp->UU.U0.ri[i] = chaves[j];
            ApTemp->UU.U0.pi[i] = filhos[j];
        }

        Ap->UU.U0.pi[M] = filhos[M];
        ApTemp->UU.U0.pi[M] = filhos[MM + 1];

        *ApRetorno = ApTemp;
        *Cresceu = true;

        return;
    }
    else
    {
        while (i < Ap->UU.U1.ne)
        {
            a->comparacoes++;
            if (Reg.chave > Ap->UU.U1.re[i].chave)
                i++;
            else
                break;
        }

        if (i < Ap->UU.U1.ne)
        {
            a->comparacoes++;
            if (Reg.chave == Ap->UU.U1.re[i].chave)
            {
                printf("Erro: registro ja esta presente.\n");
                *Cresceu = false;
                return;
            }
        }

        if (Ap->UU.U1.ne < MM2)
        {
            InsereNaPaginaEstrela(Ap, Reg, NULL, a);
            *Cresceu = false;
            return;
        }

        //Overflow: folha precisa ser dividida.
        Registros temp[MM2 + 1];

        for (j = 0; j < i; j++)
            temp[j] = Ap->UU.U1.re[j];

        temp[i] = Reg;

        for (j = i + 1; j < MM2 + 1; j++)
            temp[j] = Ap->UU.U1.re[j - 1];

        ApTemp = malloc(sizeof(TipoPaginaEstrela));

        if (ApTemp == NULL)
        {
            printf("Erro ao alocar folha.\n");
            *Cresceu = false;
            return;
        }

        ApTemp->Pt = Externa;

        Ap->UU.U1.ne = M2;

        for (i = 0, j = M2; i < M2; i++, j++)
        {
            Ap->UU.U1.re[i] = temp[i];
            ApTemp->UU.U1.re[i] = temp[j];
        }

        ApTemp->UU.U1.re[M2] = temp[MM2];
        ApTemp->UU.U1.ne = M2 + 1;

        ApTemp->UU.U1.prox = Ap->UU.U1.prox;
        ApTemp->UU.U1.ante = Ap;

        if (Ap->UU.U1.prox != NULL)
            Ap->UU.U1.prox->UU.U1.ante = ApTemp;

        Ap->UU.U1.prox = ApTemp;

        RegRetorno->chave = ApTemp->UU.U1.re[0].chave;

        *ApRetorno = ApTemp;
        *Cresceu = true;
    }
}


void InsereEstrela(Registros Reg, TipoApontadorEstrela *Ap, Analise *a)
{
    if (!*Ap)
    {
        *Ap = malloc(sizeof(TipoPaginaEstrela));

        if (*Ap == NULL)
        {
            printf("Erro ao alocar raiz inicial.\n");
            return;
        }

        (*Ap)->Pt = Externa;
        (*Ap)->UU.U1.ne = 1;
        (*Ap)->UU.U1.re[0] = Reg;
        (*Ap)->UU.U1.prox = NULL;
        (*Ap)->UU.U1.ante = NULL;

        return;
    }

    bool Cresceu;
    Registros RegRetorno;
    TipoApontadorEstrela ApRetorno, ApTemp;

    InsEstrela(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno, a);

    if (Cresceu)
    {
        ApTemp = malloc(sizeof(TipoPaginaEstrela));

        if (ApTemp == NULL)
        {
            printf("Erro ao alocar nova raiz.\n");
            return;
        }

        ApTemp->Pt = Interna;
        ApTemp->UU.U0.ni = 1;
        ApTemp->UU.U0.ri[0] = RegRetorno.chave;
        ApTemp->UU.U0.pi[1] = ApRetorno;
        ApTemp->UU.U0.pi[0] = *Ap;

        *Ap = ApTemp;
    }
}

void DestruirArvoreEstrela(TipoApontadorEstrela Ap)
{
    if (!Ap)
        return;
    if (Ap->Pt == Interna)
    {
        for (int i = 0; i <= Ap->UU.U0.ni; i++)
            DestruirArvoreEstrela(Ap->UU.U0.pi[i]);
    }
    free(Ap);
}

static void ImprimeChavesArvoreBEstrela(FILE *arqDados, long int qtd)
{
    Registros reg;

    rewind(arqDados);

    printf("\n= Chaves dos registros do arquivo =\n");

    for (long int i = 0; i < qtd && fread(&reg, sizeof(Registros), 1, arqDados) == 1; i++)
        printf("%d ", reg.chave);

    printf("\n\n");
    rewind(arqDados);
}

bool RealizaPesquisaArvoreBEstrela(Analise *a, Registros *itemPesquisado, long int qtdRegistrosArq, FILE *arqDados, bool p)
{
    if (a == NULL || itemPesquisado == NULL || arqDados == NULL)
    {
        printf("Erro: parametro invalido.\n");
        return false;
    }

    if (qtdRegistrosArq <= 0)
    {
        printf("Erro: quantidade de registros invalida.\n");
        return false;
    }

    TipoApontadorEstrela arvore;
    InicializadorEstrela(&arvore);

    if (p)
        ImprimeChavesArvoreBEstrela(arqDados, qtdRegistrosArq);

    inicializaAnalise(a);
    rewind(arqDados);

    clock_gettime(CLOCK_MONOTONIC, &a->inicio);

    Registros reg;

    for (long int i = 0; i < qtdRegistrosArq; i++)
    {
        if (fread(&reg, sizeof(Registros), 1, arqDados) != 1)
        {
            printf("Erro ao ler o registro %ld do arquivo-base.\n", i);

            CalculaTempoArvoreBEstrela(a);

            DestruirArvoreEstrela(arvore);
            return false;
        }

        a->transferencia++;

        InsereEstrela(reg, &arvore, a);
    }

    CalculaTempoArvoreBEstrela(a);

    printf("\n======== ARVORE B+ EM MEMORIA ========\n");
    printf("\n- Pre-processamento -\n");
    printf("Tempo: %.9lf segundos\n", a->tempoTotal);
    printf("Numero de comparacoes: %ld\n", a->comparacoes);
    printf("Numero de transferencias: %ld\n", a->transferencia);

    inicializaAnalise(a);
    clock_gettime(CLOCK_MONOTONIC, &a->inicio);

    bool achou = PesquisaEstrela(
        itemPesquisado->chave,
        itemPesquisado,
        &arvore,
        a
    );

    CalculaTempoArvoreBEstrela(a);

    printf("\n- Pesquisa -\n");
    printf("Tempo: %.9lf segundos\n", a->tempoTotal);
    printf("Numero de comparacoes: %ld\n", a->comparacoes);

    printf("Numero de transferencias: %ld\n", a->transferencia);

    if (achou)
    {
        printf("\nRegistro encontrado:\n");
        printf("chave = %d\n", itemPesquisado->chave);
        printf("dado1 = %ld\n", itemPesquisado->dado1);
        printf("dado2 = %s\n", itemPesquisado->dado2);
    }
    else
    {
        printf("\nA chave %d nao foi encontrada.\n",
               itemPesquisado->chave);
    }

    DestruirArvoreEstrela(arvore);

    return achou;
}

#endif