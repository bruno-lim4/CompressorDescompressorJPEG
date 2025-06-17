#ifndef CODIFICA_H
#define CODIFICA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// recebe o valor a ser codificado por diferença. altera o qtd_bits pra corresponder
// à qts de bits válidos do valor uint32_t que corresponde à codificação
uint32_t codifica_infoDC(int value, int* qtd_bits);

// recebe a tupla a ser codificada na parte de RLE. altera o qtd_bits pra corresponder
// à qts de bits válidos do valor uint32_t que corresponde à codificação
uint32_t codifica_infoAC(int qtd_zero, int value, int* qtd_bits);

#endif