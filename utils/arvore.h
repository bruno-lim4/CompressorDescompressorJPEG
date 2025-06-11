#ifndef _ARVORE_
#define _ARVORE_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct no_dc_ NO_DC;
typedef struct arvore_dc_ ARVORE_DC;
typedef struct no_ac_ NO_AC;
typedef struct arvore_ac_ ARVORE_AC;

// debug.
void printAC(ARVORE_AC* raiz);
void printDC(ARVORE_DC* raiz);


ARVORE_DC* criarArvoreDC();
NO_DC* criarNo_DC(int cat);
void inserirCatDC(ARVORE_DC* arv, int prefixo, int comprimento, NO_DC* no);
NO_DC* getRaiz_DC(ARVORE_DC* arv);
NO_DC* getFilhoEsquerdoDC(NO_DC* no);
NO_DC* getFilhoDireitoDC(NO_DC* no);
void desalocarArvoreDC(NO_DC* no);
int get_cat_DC(NO_DC* no);
int ehFolha_DC(NO_DC* no);

ARVORE_AC* criarArvoreAC();
NO_AC* criarNo_AC(int runlength, int size);
void inserirPrefixoAC(ARVORE_AC* raiz, int prefixo, int comprimento, NO_AC* no);
NO_AC* getRaiz_AC(ARVORE_AC* arv);
NO_AC* getFilhoEsquerdoAC(NO_AC* no);
NO_AC* getFilhoDireitoAC(NO_AC* no);
int ehFolha_AC(NO_AC* no);
int get_runlength_AC(NO_AC* no);
int get_size_AC(NO_AC* no);
void desalocarArvoreAC(NO_AC* no);


NO_AC* buscarPrefixo(int pref, NO_AC* atual);

#endif