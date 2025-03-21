#ifndef UTIL_H
#define UTIL_H

typedef struct bmpfileheader_ BMPFILEHEADER;
typedef struct bmpinfoheader_ BMPINFOHEADER;
typedef struct pixel_ PIXEL;

#include <stdio.h>
#include <stdlib.h>

PIXEL* criarPixel(char r, char g, char b);
PIXEL* leituraPixel(FILE* f);
void printPixel(PIXEL* pixel);

BMPFILEHEADER* criarFileHeader(unsigned short bfType, unsigned int bfSize, unsigned short bfReserved1, unsigned short bfReserved2, unsigned int bfOffBits);
BMPFILEHEADER* leituraFileHeader(FILE* arq);
void printFileHeader(BMPFILEHEADER* fileHeader);

BMPINFOHEADER* leituraInfoHeader(FILE* arq);
BMPINFOHEADER* criarInfoHeader(unsigned int biSize, int biWidth, int biHeight, unsigned short biPlanes, unsigned short biBitCount, unsigned int biCompression, unsigned int biSizeImage, int biXPelsPerMeter, int biYPelsPerMeter, unsigned int biClrUsed, unsigned int biClrImportant);
void printInfoHeader(BMPINFOHEADER* infoHeader);


#endif