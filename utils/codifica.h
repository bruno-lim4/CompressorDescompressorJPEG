#ifndef CODIFICA_H
#define CODIFICA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// relacionados a gravar dados
uint32_t codifica_infoDC(int value, int* qtd_bits);
uint32_t codifica_infoAC(int qtd_zero, int value, int* qtd_bits);

#endif