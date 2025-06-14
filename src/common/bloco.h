#ifndef BLOCO_H
#define BLOCO_H

//BLOCOS 8X8
typedef struct bloco_ BLOCO;

#include <stdio.h>
#include <stdlib.h>

double getPosBloco(BLOCO *b, int i, int j);
BLOCO* criarBloco(double** pos, int start_i, int start_j, char tipo);
void printBloco(BLOCO *b);
void desalocarBloco(BLOCO** bloco);
BLOCO* aplicaDCT(BLOCO* bloco);
BLOCO* desfazDCT(BLOCO* bloco);
BLOCO* aplicaQuantizacao(BLOCO* bloco);
BLOCO* desfazQuantizacao(BLOCO* blocoQuantizado);
int* pega_zigzag(BLOCO* bloco);
BLOCO* monta_bloco(int* vetor, char tipo);
void gravaBloco(double **m, int start_i, int start_j, BLOCO *b);
int* processaBloco(BLOCO* bloco);

#endif