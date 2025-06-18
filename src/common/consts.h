#ifndef CONSTS_H
#define CONSTS_H

// define algumas constantes importantes
#include <stdint.h>

// matriz DCT
extern const double matriz_DCT[8][8];

// matriz DCT transposta
extern const double matriz_DCT_T[8][8];

// inversa da matriz DCT
extern const double matriz_DCT_INV[8][8];

// inversa da matriz transposta da DCT
extern const double matriz_DCT_T_INV[8][8];

// matriz usada na quantizacao de blocos de luminancia
extern const int quant_luminancia[8][8];

// matriz usada na quantizacao de blocos de crominancia
extern const int quant_crominancia[8][8];

// tabela de Huffman
extern const uint32_t tabelaAC[16][10];

// valor caso tenha 15 zeros seguidos quando codifica AC
extern const uint32_t tabelaAC_quinzeZeros;

// valor caso acabe o bloco quando codificado AC
extern const uint32_t tabelaAC_acabouVetor;

// valores para achar a categoria quando codificar AC
extern const uint32_t tabelaDC[11];

#endif