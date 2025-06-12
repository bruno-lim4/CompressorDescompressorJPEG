#ifndef HEADER_H
#define HEADER_H

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
void escreverFileHeader(FILE* arq, BMPFILEHEADER* fileHeader);
void desalocarFileHeader(BMPFILEHEADER** fileHeader);
void printFileHeader(BMPFILEHEADER* fileHeader);

BMPINFOHEADER* leituraInfoHeader(FILE* arq);
BMPINFOHEADER* criarInfoHeader(unsigned int biSize, int biWidth, int biHeight, unsigned short biPlanes, unsigned short biBitCount, unsigned int biCompression, unsigned int biSizeImage, int biXPelsPerMeter, int biYPelsPerMeter, unsigned int biClrUsed, unsigned int biClrImportant);
void escreverInfoHeader(FILE* arq, BMPINFOHEADER* infoHeader);
int get_biWidth(BMPINFOHEADER* infoHeader);
int get_biHeight(BMPINFOHEADER* infoHeader);
void desalocarInfoHeader(BMPINFOHEADER** infoHeader);
void printInfoHeader(BMPINFOHEADER* infoHeader);

#endif