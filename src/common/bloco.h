#ifndef BLOCO_H
#define BLOCO_H

//BLOCOS 8X8
typedef struct bloco_ BLOCO;

#include <stdio.h>
#include <stdlib.h>

// cria um bloco 8x8 com valores identicos ao sub-bloco 8x8 da matriz pos com inicio na posicao
// (start_i, start_j) - que é canto superior esquerdo do sub-bloco
// tipo é de luminancia (Y) ou crominancia (B pra azul e R pra vermelho)
BLOCO* criarBloco(double** pos, int start_i, int start_j, char tipo);

// desaloca bloco
void desalocarBloco(BLOCO** bloco);

// aplica DCT e retorna o bloco com os valores da DCT
BLOCO* aplicaDCT(BLOCO* bloco);

// aplica a inversa da DCT e retorna o bloco com os valores obtidos
BLOCO* desfazDCT(BLOCO* bloco);

// aplica quantização com paramentro k = 1 e retorna o novo bloco
BLOCO* aplicaQuantizacao(BLOCO* bloco);

// desfaz quantização com parâmetro k = 1 e retorna o novo bloco
BLOCO* desfazQuantizacao(BLOCO* blocoQuantizado);

// pega um vetor de 64 posicoes baseando-se no bloco 8x8
int* pega_zigzag(BLOCO* bloco);

// Recebe um vetor e o transforma num bloco 8x8.
BLOCO* monta_bloco(int* vetor, char tipo);

// Guarda na matriz 'm' os valores do bloco 'b', começando na posição [start_i][start_j].
void gravaBloco(double **m, int start_i, int start_j, BLOCO *b);

// aplica DCT e quantizacao no bloco, e retorna o vetor zig-zag correspondente
int* processaBloco(BLOCO* bloco);

#endif