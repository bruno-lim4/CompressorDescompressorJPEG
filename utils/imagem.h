#ifndef IMAGEM_H
#define IMAGEM_H

typedef struct imagem_ IMAGEM;

#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include "util.h"

IMAGEM* criarImagem(FILE* f);
void printarImagem(IMAGEM* img);
void escreverImagem(FILE* f, IMAGEM* img);
void comprimeImagem(IMAGEM* img, FILE* f);

#endif