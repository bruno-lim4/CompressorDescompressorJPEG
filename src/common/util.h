#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>

void alocarMatriz_double(double*** m, int h, int w);
void alocarMatriz_unsignedChar(unsigned char*** m, int h, int w);
double** matrizTransposta(double** a, int n);
double** mult_matrizQuadrada(double** a, double** b, int n);
double** mult_constMatriz(const double a[8][8], double** b, int n);
double** mult_matrizConst(double** a, const double b[8][8], int n);
void desalocarMatriz_double(double ***m, int h);
void desalocarMatriz_int(int ***m, int h);
void desalocarMatriz_unsignedChar(unsigned char ***m, int h);

void print_binary(int value, int bits);

// shifta "a" para a esquerda e grava "b" dentro dela (os qtd_b bits menos significativos de "b")
uint32_t shifta_e_grava(uint32_t a, uint32_t b, int qtd_b);

// pega o value, aplica complemento de 1, e retorna o valor um uint32_t, modificando qtd
// para ser o numero de bits menos significativos validos do valor retornado
uint32_t get_mantissa_comp1(int value, int* qtd);

int get_qtdBits(uint32_t value);

void printMatrizUchar(unsigned char** m, int h, int w);
void printMatrizDouble(double** m, int h, int w);

#endif