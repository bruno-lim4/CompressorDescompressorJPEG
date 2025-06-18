#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>

// aloca matriz double com h linhas e w colunas no ponteiro triplo passado como parametro
void alocarMatriz_double(double*** m, int h, int w);
// aloca matriz unsignedChar com h linhas e w colunas no ponteiro triplo passado como parametro
void alocarMatriz_unsignedChar(unsigned char*** m, int h, int w);

// calcula a matriz transposta de uma matriz double quadrada de n linhas e retorna ela
double** matrizTransposta(double** a, int n);

// multiplica duas matrizes double quadrada de lado n e retorna o resultado
double** mult_matrizQuadrada(double** a, double** b, int n);
// multiplica uma matriz const double com uma matriz double, ambos quadrada de lado n, e retorna o resultado
double** mult_constMatriz(const double a[8][8], double** b, int n);
// multiplica uma matriz double com uma matriz const double, ambos quadrada de lado n, e retorna o resultado
double** mult_matrizConst(double** a, const double b[8][8], int n);

// delasola matriz double com h linhas
void desalocarMatriz_double(double ***m, int h);
// desaloca matriz int com h linhas
void desalocarMatriz_int(int ***m, int h);
// desaloca matriz unsigned char com h linhas
void desalocarMatriz_unsignedChar(unsigned char ***m, int h);

// shifta "a" para a esquerda e grava "b" dentro dela (os qtd_b bits menos significativos de "b")
uint32_t shifta_e_grava(uint32_t a, uint32_t b, int qtd_b);

// pega o value, aplica complemento de 1, e retorna o valor um uint32_t, modificando qtd
// para ser o numero de bits menos significativos validos do valor retornado
uint32_t get_mantissa_comp1(int value, int* qtd);

// pega a qtd de bits de um valor. Isso é: a qtd de bits menos significativos até o 1 mais significativo
int get_qtdBits(uint32_t value);

#endif