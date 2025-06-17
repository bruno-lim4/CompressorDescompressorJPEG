#ifndef CONSTS_H
#define CONSTS_H

// define algumas constantes importantes
#include <stdint.h>

extern const double matriz_DCT[8][8];
extern const double matriz_DCT_T[8][8];
extern const double matriz_DCT_INV[8][8];
extern const double matriz_DCT_T_INV[8][8];
extern const int quant_luminancia[8][8];
extern const int quant_crominancia[8][8];
extern const uint32_t tabelaAC[16][10];
extern const uint32_t tabelaAC_quinzeZeros;
extern const uint32_t tabelaAC_acabouVetor;

#endif