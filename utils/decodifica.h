#ifndef _DECODIFICA_
#define _DECODIFICA_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "leitor.h"

void decodificaDC(int* DC, LEITOR* l, int DC_anterior);
void decodificaAC(int *bloco, LEITOR* l);

#endif