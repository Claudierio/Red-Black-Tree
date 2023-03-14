#include <stdio.h>
#include <stdlib.h>
#include"rubroNegra.h"

No* criar_no(int info){
    No*novo = (No*)malloc(sizeof(No));
    novo->info = info;
    novo->pai = NULL;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->cor = VERMELHO;
}

void pre_order(No* raiz){
   if(raiz == NULL) ;
   else {
      if(raiz->cor == PRETO)
      printf("\x1b[30m[%d]\x1b[0m", raiz->info);
      else
      printf("\x1b[31m[%d]\x1b[0m", raiz->info);
      pre_order(raiz->esq);
      pre_order(raiz->dir);
   }
}

int cor(No* no){
    if(no == NULL || no-> cor == PRETO)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int eh_esquerdo(No* no){
    if(no == no->pai->esq) {
        return 1;
    }
    else{
        return 0;
    }
}

int eh_raiz(No* no){
    if(no->pai == NULL){
        return 1;
    }
    else{
        return 0;
    }
}

No* tio(No* n){
    if(eh_esquerdo(n->pai)){
        return n->pai->pai->dir;
    }
    else{
        return n->pai->pai->esq;
    }
}

void rotacao_simples_dir(No **raiz, No* no, int simples){
    No *p, *t;

    p = no->esq;
    t = p->dir;

    no->esq = t;
    p->dir = no;

    if(simples){
        p->cor = PRETO;
        no->cor = VERMELHO;
    }

    if(t != NULL){
        t->pai = no;
        p->pai = no->pai;
    }

    if(eh_raiz(no)){
        *raiz = p;
    }

    else{
        if(eh_esquerdo(no)){
            no->pai->esq = p;
        }
        else{
            no->pai->dir = p;
        }
    }
    no->pai = p;
}

void rotacao_simples_esq(No** raiz, No* no, int simples){
   No *p, *t;

   p = no->dir;
   t = p->esq;

   no->dir = t;
   p->esq = no;

   if(simples){
      p->cor = PRETO;
      no->cor = VERMELHO;
   }

   if(t != NULL){
      t->pai = no;
   }

   p->pai = no->pai;

   if(eh_raiz(no)){
      *raiz = p;
   }

   else {
      if(eh_esquerdo(no))
      {
         no->pai->esq = p;
      } 
      
      else 
      {
         no->pai->dir = p;
      }
   }
   no->pai = p;

}

void rotacaoDupla_esq(No** raiz, No* no){
    rotacao_simples_dir(raiz, no->dir, 0);
    rotacao_simples_esq(raiz, no, 0);

    no->pai->cor = PRETO;
    no->cor = VERMELHO;
}

void rotacaoDupla_dir(No** raiz, No* no){
    rotacao_simples_esq(raiz, no->esq, 0);
    rotacao_simples_dir(raiz, no, 0);

    no->pai->cor = PRETO;
    no->cor = VERMELHO;
}

void ajustar(No** raiz, No* no){
    if(eh_raiz(no)){
        no->cor = PRETO;
        return;
    }
    else if(no->pai->cor == VERMELHO){
        //Caso 1: Tio vermelho, baixa o preto do avo
        if(cor(tio(no)) == VERMELHO){
            no->pai->cor = PRETO;
            tio(no)->cor = PRETO;
            no->pai->pai->cor = VERMELHO;
            ajustar(raiz, no->pai->pai);
        }
        //caso 2: Tio preto
        else{
            //Nó filho e no pai, filhos esquerdos
            if(eh_esquerdo(no) && eh_esquerdo(no->pai)){ 
                rotacao_simples_dir(raiz, no-> pai, 1);
            }
            //NO FILHO E NO PAI, FILHOS DIREITOS
            else if(!eh_esquerdo(no) && !eh_esquerdo(no->pai)){
                rotacao_simples_esq(raiz, no->pai->pai, 1);
            }

            //NO FILHO ESQ, PAI DIR
            else if(eh_esquerdo(no) && !eh_esquerdo(no->pai)){
                rotacaoDupla_esq(raiz, no->pai->pai);
            }

            //NO FILHO DIR, PAI ESQ;
            else if(!eh_esquerdo(no) && eh_esquerdo(no->pai)){
                rotacaoDupla_dir(raiz, no->pai->pai);
            }
        }
    }
}

void inserir(No** raiz, int info){
    No *temp, *pai, *novo;
    temp = *raiz;
    pai = NULL;

    while(temp != NULL){
        pai = temp;

        if(info > temp->info){
            temp = temp->dir;
        }

        else{
            temp = temp->esq;
        }
    }

    novo = criar_no(info);
    novo->pai = pai;

    if(eh_raiz(novo)){
        *raiz = novo;
    }
    else if(info < pai->info){
        pai->esq = novo;
    }
    else{
        pai->dir = novo;
    }
    ajustar(raiz, novo);
}