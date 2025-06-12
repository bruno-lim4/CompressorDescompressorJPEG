#ifndef _DECODIFICA_
#define _DECODIFICA_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "leitor.h"

void iniciarDecodificacao();
void decodificaDC(int* DC, LEITOR* l, int DC_anterior, int* ehPrimeiroDC);
void decodificaAC(int *bloco, LEITOR* l);
void encerrarDecodificacao();

#endif