#include "bloco.h"
#include "util.h"
#include "consts.h"

// BLOCOS 8X8
struct bloco_ {
    double** m;
    char tipo; // L para luminancia, B para Cb, e R para CR
};

BLOCO* criarBloco(double** pos, char tipo) {
    BLOCO* bloco = (BLOCO*) malloc(sizeof(BLOCO));
    alocarMatriz_double(&(bloco->m), 8, 8);

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            bloco->m[i][j] = pos[i][j];
        }
    }

    bloco->tipo = tipo;

    return bloco;
}

void desalocarBloco(BLOCO** bloco) {
    desalocarMatriz_double(&((*bloco)->m), 8, 8);
    free(*bloco);
}

BLOCO* aplicaDCT(BLOCO* bloco) {
    double** DCT = mult_matrizConst(mult_constMatriz(matriz_DCT, bloco->m, 8), matriz_DCT_T, 8);
    return criarBloco(DCT, bloco->tipo);
}

BLOCO* desfazDCT(BLOCO* bloco) {
    double** res = mult_matrizConst(mult_constMatriz(matriz_DCT_INV, bloco->m,8), matriz_DCT_T_INV, 8);
    return criarBloco(res, bloco->tipo);
}

BLOCO* aplicaQuantizacao(BLOCO* bloco){
    BLOCO* blocoQuantizado = criarBloco(bloco->m, bloco->tipo);
    int k = 1; //taxa de compressão

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(blocoQuantizado->tipo == 'L')
                (blocoQuantizado->m)[i][j] /= k*quant_luminancia[i][j];
            else
                (blocoQuantizado->m)[i][j] /= k*quant_crominancia[i][j];
        }
    }

    return blocoQuantizado;
}