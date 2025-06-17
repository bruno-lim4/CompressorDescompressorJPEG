#ifndef GRAVADOR_H
#define GRAVADOR_H

typedef struct gravador_ GRAVADOR;

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// cria um GRAVADOR que gravará bytes no arquivo f usando um buffer interno
GRAVADOR* criarGravador(FILE* f);

// desaloca GRAVADOR
void desalocarGravador(GRAVADOR** gravador);

// grava value no arquivo do GRAVADOR. Para isso, recebe o qtd_bits que
// seria a qtd de bits menos significativos de value a serem considerados
// para a gravacao
void gravarValor(GRAVADOR* gravador, uint32_t value, int qtd_bits);

// finaliza a gravacao (limpa o buffer interno e grava no arquivo)
void finalizarGravacao(GRAVADOR* gravador);

#endif