#ifndef UTIL_H
#define UTIL_H

typedef struct bmpfileheader_ BMPFILEHEADER;

typedef struct bmpinfoheader_ BMPINFOHEADER;

#include <stdio.h>
#include <stdlib.h>

BMPFILEHEADER* criarFileHeader(unsigned short bfType, unsigned int bfSize, unsigned short bfReserved1, unsigned short bfReserved2, unsigned int bfOffBits);
BMPFILEHEADER* leituraFileHeader(FILE* arq);

#endif