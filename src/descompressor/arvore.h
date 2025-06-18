#ifndef _ARVORE_
#define _ARVORE_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Definição das structs usadas para construir as árvores de prefixos.
typedef struct no_dc_ NO_DC;
typedef struct arvore_dc_ ARVORE_DC;
typedef struct no_ac_ NO_AC;
typedef struct arvore_ac_ ARVORE_AC;

// Cria a árvore de prefixos DC, armazenando-a numa variável global.
ARVORE_DC* criarArvoreDC();
// Cria um nó com a categoria igual a 'cat'.
NO_DC* criarNo_DC(int cat); 
// Insere um nó com categoria 'cat' na árvore.
void inserirCatDC(ARVORE_DC* arv, int prefixo, int comprimento, int cat); 
// Retorna o nó raíz da árvore.
NO_DC* getRaiz_DC(ARVORE_DC* arv); 
// Retorna o filho esquerdo do nó.
NO_DC* getFilhoEsquerdoDC(NO_DC* no);
// Retorna o filho direito do nó. 
NO_DC* getFilhoDireitoDC(NO_DC* no);
// Retorna a categoria armazenada no nó.
int get_cat_DC(NO_DC* no);
// Retorna se o nó é um nó folha.
int ehFolha_DC(NO_DC* no);
// libera toda a memória alocada para a árvore.
void desalocarArvoreDC(ARVORE_DC** no);

// Cria a árvore de prefixos AC, armazenando-a numa variável global.
ARVORE_AC* criarArvoreAC();
// Cria um nó guardando as informações: número de zeros igual a 'runlength' e tamanho do valor igual a 'size'.
NO_AC* criarNo_AC(int runlength, int size);
// Insere um nó com os valores de 'runlength' e 'size' na árvore.
void inserirPrefixoAC(ARVORE_AC* raiz, int prefixo, int comprimento, int runlength, int size);
// Retorna a raíz da árvore.
NO_AC* getRaiz_AC(ARVORE_AC* arv);
// Retorna o filho esquerdo do nó.
NO_AC* getFilhoEsquerdoAC(NO_AC* no);
// Retorna o filho direito do nó.
NO_AC* getFilhoDireitoAC(NO_AC* no);
// Retorna o valor de 'runlength' armazenado no nó.
int get_runlength_AC(NO_AC* no);
// Retorna o valor de 'size' armazenado no nó.
int get_size_AC(NO_AC* no);
// Retorna se o nó é um nó folha.
int ehFolha_AC(NO_AC* no);
// Libera toda a memória alocada para a árvore.
void desalocarArvoreAC(ARVORE_AC** no);

#endif