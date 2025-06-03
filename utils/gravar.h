#ifndef GRAVAR_H
#define GRAVAR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// relacionados a gravar dados
uint32_t grava_infoDC(int value, int* qtd_bits);
uint32_t grava_infoAC(int qtd_zero, int value, int* qtd_bits);
void print_binary(int value, int bits);

#endif