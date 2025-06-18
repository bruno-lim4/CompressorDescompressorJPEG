#ifndef _LEITOR_
#define _LEITOR_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct leitor_ LEITOR;

// Cria a estrutura que lê os bits do arquivo de entrada.
LEITOR* criarLeitor(FILE *f);
// Carrega o buffer do leitor com bytes do arquivo de entrada.
void carregarBuffer(LEITOR* l);
// Lê o próximo bit do arquivo, isto é, retorna o bit menos significativo armazenado no buffer.
int lerBit(LEITOR *l);
// Lê um valor de comprimento 'numBits'. Verifica se é negativo (olhando se o bit mais significativo é 0)
// e, se for, desfaz o complemento de 1, e retorna o valor.
int lerValor(int numBits, LEITOR* l);
// Liberação de memória.
void destruirLeitor(LEITOR** l);

#endif