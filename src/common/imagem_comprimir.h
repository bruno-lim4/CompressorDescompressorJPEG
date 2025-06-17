#ifndef IMAGEM_COMPRESS_H
#define IMAGEM_COMPRESS_H

#include "imagem.h"
#include "../compressor/codifica.h"

// pega uma imagem e comprime ela em um arquivo f, que será um .bin
void comprimeImagem(IMAGEM* img, FILE* f);

#endif