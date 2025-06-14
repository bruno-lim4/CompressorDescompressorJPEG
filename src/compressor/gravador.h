#ifndef GRAVADOR_H
#define GRAVADOR_H

typedef struct gravador_ GRAVADOR;

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

GRAVADOR* criarGravador(FILE* f);
void desalocarGravador(GRAVADOR** gravador);
void gravarValor(GRAVADOR* gravador, uint32_t value, int qtd_bits);
void finalizarGravacao(GRAVADOR* gravador);

#endif