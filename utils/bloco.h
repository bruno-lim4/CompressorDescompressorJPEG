#ifndef BLOCO_H
#define BLOCO_H

//BLOCOS 8X8
typedef struct bloco_ BLOCO;

#include <stdio.h>
#include <stdlib.h>

BLOCO* criarBloco(double** pos, char tipo);
void desalocarBloco(BLOCO** bloco);
BLOCO* aplicaDCT(BLOCO* bloco);
BLOCO* desfazDCT(BLOCO* bloco);
BLOCO* aplicaQuantizacao(BLOCO* bloco);

#endif