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
void comprimeImagem(IMAGEM* img);
IMAGEM* construirImagem(double ***blocos, int img_h, int img_w);
void printarImagem(IMAGEM* img);

#endif