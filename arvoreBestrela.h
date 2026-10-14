#ifndef arvoreBestrela_h
#define arvoreBestrela_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "base.h"

#define M 2
#define MM 4
#define M2 2
#define MM2 4

typedef struct TipoPagina* TipoApontador;

typedef enum {Interna, Externa} TipoIntExt;

typedef struct TipoPagina { 
   TipoIntExt Pt; 
   union 
   { 
      struct 
      { 
        int ni; 
        int ri[MM];
        TipoApontador pi[MM + 1]; 
      } U0; 
      struct 
      { 
        int ne; 
        registros re[MM2]; 
        TipoApontador prox;
        TipoApontador ante;
      } U1; 
   } UU; 
} TipoPagina;

void inicializadorestrela(TipoApontador *arvore){
    *arvore = NULL;
}

bool Pesquisaestrela(long Ch, registros *x, TipoApontador *Ap)
{
    int i;
    TipoApontador Pag = *Ap;
    if (Pag == NULL)
    {
        printf("Árvore não existe/vazia\n");
        return false;
    }
    if (Pag->Pt == Interna)
    {
        i = 1;
        while (i <= Pag->UU.U0.ni && Ch >= Pag->UU.U0.ri[i-1])
            i++;
        return Pesquisaestrela(Ch, x, &Pag->UU.U0.pi[i-1]);
    }
    i = 1;
    while (i < Pag->UU.U1.ne && Ch > Pag->UU.U1.re[i-1].chave)
        i++;
    if (Ch == Pag->UU.U1.re[i-1].chave)
    {
        printf("Registro encontrado com sucesso!\n");
        *x = Pag->UU.U1.re[i-1];
        return true;
    }
    printf("Registro não encontrado na árvore.\n");
    return false;
}

void InsereNaPaginaestrela(TipoApontador Ap, registros Reg, TipoApontador ApDir)
{ 
    bool NaoAchouPosicao;
    int k;
    if(Ap->Pt == Interna)
    {
        k = Ap->UU.U0.ni; 
        NaoAchouPosicao = (k > 0); 
        while (NaoAchouPosicao)
        { 
            if (Reg.chave >= Ap->UU.U0.ri[k-1]) 
            { 
                NaoAchouPosicao = false;
                break;
            }
            Ap->UU.U0.ri[k] = Ap->UU.U0.ri[k-1]; 
            Ap->UU.U0.pi[k+1] = Ap->UU.U0.pi[k];
            k--;
            if (k < 1) 
                NaoAchouPosicao = false;
        }
        Ap->UU.U0.ri[k] = Reg.chave;
        Ap->UU.U0.pi[k+1] = ApDir;
        Ap->UU.U0.ni++;
    }
    else
    {
        k = Ap->UU.U1.ne; 
        NaoAchouPosicao = (k > 0); 
        while (NaoAchouPosicao)
        { 
            if (Reg.chave >= Ap->UU.U1.re[k-1].chave) 
            { 
                NaoAchouPosicao = false;
                break;
            }
            Ap->UU.U1.re[k] = Ap->UU.U1.re[k-1]; 
            k--;
            if (k < 1) 
                NaoAchouPosicao = false;
        }
        Ap->UU.U1.re[k] = Reg;
        Ap->UU.U1.ne++;
    }
}

void Insestrela(registros Reg, TipoApontador Ap, bool *Cresceu, registros *RegRetorno, TipoApontador *ApRetorno)
{ 
    long i = 1; 
    long j;
    TipoApontador ApTemp;
    if(Ap->Pt == Interna)
    {
        while (i < Ap->UU.U0.ni && Reg.chave > Ap->UU.U0.ri[i - 1])  
            i++;
        if (Reg.chave < Ap->UU.U0.ri[i - 1]) 
            i--;
        Insestrela(Reg, Ap->UU.U0.pi[i], Cresceu, RegRetorno, ApRetorno);
        if (!*Cresceu) 
            return;
        if (Ap->UU.U0.ni < MM)   /* Pagina tem espaco */
        { 
            InsereNaPaginaestrela(Ap, *RegRetorno, *ApRetorno);
            *Cresceu = false;
            return;
        }

        /* Overflow: Pagina tem que ser dividida */
        ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
        ApTemp->Pt = Interna;
        ApTemp->UU.U0.ni = 0;  
        ApTemp->UU.U0.pi[0] = NULL;
        if (i < M + 1)
        { 
            InsereNaPaginaestrela(ApTemp, Ap->UU.U0.ri[MM - 1], Ap->UU.U0.pi[MM]);
            Ap->UU.U0.ni--;
            InsereNaPaginaestrela(Ap, *RegRetorno, *ApRetorno);
        }
        else 
            InsereNaPaginaestrela(ApTemp, *RegRetorno, *ApRetorno);
        for (j = M + 2; j <= MM; j++)
        {
            InsereNaPaginaestrela(ApTemp, Ap->UU.U0.ri[j-1], Ap->UU.U0.pi[j]);
        }
        Ap->UU.U0.ni = M;  
        ApTemp->UU.U0.pi[0] = Ap->UU.U0.pi[M+1];
        RegRetorno->chave = Ap->UU.U0.ri[M];  
        *ApRetorno = ApTemp;
    }
    else
    {
        while (i < Ap->UU.U1.ne && Reg.chave > Ap->UU.U1.re[i - 1])  
            i++;
        if (i > 0 && Reg.chave == Ap->UU.U1.re[i-1].chave)
        { 
            printf(" Erro: Registro ja está presente\n"); 
            *Cresceu = false;
            return;
        }
        if (i > 0 && Reg.chave < Ap->UU.U1.re[i-1].chave) 
            i--;
        if (Ap->UU.U1.ne < MM2)   /* Pagina tem espaco */
        { 
            InsereNaPaginaestrela(Ap, Reg, NULL);
            *Cresceu = false;
            return;
        }
        
        /* Overflow: Pagina tem que ser dividida */
        *Cresceu = true;
        ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
        ApTemp->Pt = Externa;
        ApTemp->UU.U1.ne = 0;  
        ApTemp->UU.U1.prox = Ap->UU.U1.prox;
        ApTemp->UU.U1.ante = Ap;
        if(Ap->UU.U1.prox)
            Ap->UU.U1.prox->UU.U1.ante = ApTemp;
        Ap->UU.U1.prox = ApTemp;
        if (i < M2 + 1)
        { 
            InsereNaPaginaestrela(ApTemp, Ap->UU.U1.re[MM2-1], NULL);
            Ap->UU.U1.ne--;
            InsereNaPaginaestrela(Ap, Reg, NULL);
        }
        else 
            InsereNaPaginaestrela(ApTemp, Reg, NULL);
        for (j = M2 + 1; j <= MM2; j++)
            InsereNaPaginaestrela(ApTemp, Ap->UU.U1.re[j-1], NULL);
        Ap->UU.U1.ne = M2;  
        *RegRetorno = ApTemp->UU.U1.re[0];  
        *ApRetorno = ApTemp;
    }
}

void Insereestrela(registros Reg, TipoApontador *Ap)
{
    if(!*Ap)
    {
        *Ap = (TipoApontador)malloc(sizeof(TipoPagina));
        (*Ap)->Pt = Externa;
        (*Ap)->UU.U1.ne = 1;
        (*Ap)->UU.U1.re[0] = Reg;
        (*Ap)->UU.U1.prox = NULL;
        (*Ap)->UU.U1.ante = NULL;
        return;
    }
    bool Cresceu;
    registros RegRetorno;
    TipoApontador ApRetorno, ApTemp;
    Insestrela(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);
    if (Cresceu)
    {
        ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
        ApTemp->Pt = Interna;
        ApTemp->UU.U0.ni = 1;
        ApTemp->UU.U0.ri[0] = RegRetorno.chave;
        ApTemp->UU.U0.pi[1] = ApRetorno;
        ApTemp->UU.U0.pi[0] = *Ap;  
        *Ap = ApTemp;
    }
}

void DestruirArvoreestrela(TipoApontador Ap)
{
    if (!Ap) 
        return;
    if (Ap->Pt == Interna)
    {
        for (int i = 0; i <= Ap->UU.U0.ni; i++)
            DestruirArvoreestrela(Ap->UU.U0.pi[i]);
    }
    free(Ap);
}

#endif