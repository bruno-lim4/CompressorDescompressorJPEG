#ifndef IMAGEM_DECOMPRESS_H
#define IMAGEM_DECOMPRESS_H

#include "imagem.h"
#include "../descompressor/decodifica.h"

// Recebe o arquivo comprimido e retorna uma struct IMAGEM com a imagem descomprimida.
IMAGEM* descomprimeImagem(FILE* in); 

#endif