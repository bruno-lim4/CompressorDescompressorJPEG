#ifndef IMAGEM_H
#define IMAGEM_H

typedef struct imagem_ IMAGEM;

#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include "util.h"

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

// pega um arquivo .bmp e gera um TAD IMAGEM, preenchendo o YCbCr já com downsampling
IMAGEM* criarImagem(FILE* f);

// salva o TAD Imagem em um arquivo .bmp
void salvarImagem(FILE* f, IMAGEM* img);

// desaloca IMAGEM
void desalocarImagem(IMAGEM** img);

#endif