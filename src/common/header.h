#ifndef HEADER_H
#define HEADER_H

typedef struct bmpfileheader_ BMPFILEHEADER;
typedef struct bmpinfoheader_ BMPINFOHEADER;

#include <stdio.h>
#include <stdlib.h>

// lê o FileHeader de um arquivo .bmp e retorna um TAD
BMPFILEHEADER* leituraFileHeader(FILE* arq);
// escreve o FileHeader em um arquivo arq
void escreverFileHeader(FILE* arq, BMPFILEHEADER* fileHeader);
// desaloca FileHeader
void desalocarFileHeader(BMPFILEHEADER** fileHeader);

// lê o InfoHeader de um arquivo .bmp e retorna um TAD
BMPINFOHEADER* leituraInfoHeader(FILE* arq);
// escreve o InfoHeader em um arquivo arq
void escreverInfoHeader(FILE* arq, BMPINFOHEADER* infoHeader);
// pega a largura em pixels. Informacao contida no infoHeader
int get_biWidth(BMPINFOHEADER* infoHeader);
// pega a altura em pixels. Informacao contida no infoHeader
int get_biHeight(BMPINFOHEADER* infoHeader);
// desaloca infoHeader
void desalocarInfoHeader(BMPINFOHEADER** infoHeader);

// checa se dimensao eh multiplo de 8, bits por pixel eh 24, e cabecalho tem 54 bytes.
// encerra a execucao do programa (com msg de erro) caso contrario
int checaInfoFileHeader(BMPFILEHEADER* fileHeader, BMPINFOHEADER* infoHeader);

#endif