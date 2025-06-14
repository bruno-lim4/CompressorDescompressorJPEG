#ifndef IMAGEM_H
#define IMAGEM_H

typedef struct imagem_ IMAGEM;

#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include "util.h"

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

IMAGEM* criarImagem(FILE* f);
void salvarImagem(FILE* f, IMAGEM* img);
void desalocarImagem(IMAGEM** img);

#endif