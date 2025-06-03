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
void desalocarMatriz_double(double ***m, int h , int w);
void desalocarMatriz_unsignedChar(unsigned char ***m, int h , int w);

// relacionados a gravar dados

uint32_t grava_infoDC(int value, int* qtd_bits);
uint32_t grava_infoAC(int value);

#endif