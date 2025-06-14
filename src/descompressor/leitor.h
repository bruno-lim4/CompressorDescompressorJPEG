#ifndef _LEITOR_
#define _LEITOR_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct leitor_ LEITOR;

LEITOR* criarLeitor(FILE *f);
void carregarBuffer(LEITOR* l);
int lerBit(LEITOR *l);
int lerValor(int numBits, LEITOR* l);
void destruirLeitor(LEITOR** l);

#endif