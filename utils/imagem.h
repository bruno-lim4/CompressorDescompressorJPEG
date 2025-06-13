#ifndef IMAGEM_H
#define IMAGEM_H

typedef struct imagem_ IMAGEM;

#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include "util.h"

int get_img_w(IMAGEM *img);
IMAGEM* criarImagem(FILE* f);
void printarImagem(IMAGEM* img);
void escreverImagem(FILE* f, IMAGEM* img);
void comprimeImagem(IMAGEM* img, FILE* f);
IMAGEM* descomprimeImagem(FILE* in, FILE* out);
void printarImagem(IMAGEM* img);
void desalocarImagem(IMAGEM** img);
void salvarImagem(FILE* f, IMAGEM* img);
//função de debug.
void printarInt(int byte);
void printarLongInt(long int byte);
void printarUint_32(uint32_t buffer);
int calcularCompInt(int pref);

#endif