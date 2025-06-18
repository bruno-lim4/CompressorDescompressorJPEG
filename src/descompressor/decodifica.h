#ifndef _DECODIFICA_
#define _DECODIFICA_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "leitor.h"
// Cria as árvores de prefixos AC e DC usadas na decodificação.
void iniciarDecodificacao();
// Decodifica o valor da diferença entre componentes DC comprimida no arquivo
// e soma-a ao valor de DC_anterior, armazenando o resultado em DC. Se ehPrimeiroDC == 1,
// simplesmente armazena o valor da diferença em DC.
void decodificaDC(int* DC, LEITOR* l, int DC_anterior, int* ehPrimeiroDC);
// Decodifica os 63 coeficientes AC e os armazena no vetor "bloco", nas posições de 1 a 63 (na 0 tem o DC).
void decodificaAC(int *bloco, LEITOR* l);
// Desaloca as árvores de prefixos.
void encerrarDecodificacao();

#endif