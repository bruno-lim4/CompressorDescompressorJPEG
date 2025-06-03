#include "bloco.h"
#include "util.h"
#include "consts.h"

// BLOCOS 8X8
struct bloco_ {
    double** m;
    char tipo; // L para luminancia, B para Cb, e R para CR
};

BLOCO* criarBloco(double** pos, int start_i, int start_j, char tipo) {
    // cria e aloca BLOCO 8x8
    BLOCO* bloco = (BLOCO*) malloc(sizeof(BLOCO));
    alocarMatriz_double(&(bloco->m), 8, 8);

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            bloco->m[i][j] = pos[start_i+i][start_j+j];
        }
    }

    bloco->tipo = tipo;

    return bloco;
}

void printBloco(BLOCO *b){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            printf("%3lf ", (b->m)[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

void desalocarBloco(BLOCO** bloco) {
    desalocarMatriz_double(&((*bloco)->m), 8, 8);
    free(*bloco);
}

// faz o level sampling tbm
BLOCO* aplicaDCT(BLOCO* bloco) {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            (bloco->m)[i][j] -= 128;
        }
    }
    double** DCT = mult_matrizConst(mult_constMatriz(matriz_DCT, bloco->m, 8), matriz_DCT_T, 8);
    return criarBloco(DCT, 0, 0, bloco->tipo);
}

// desfaz o level sampling tbm
BLOCO* desfazDCT(BLOCO* bloco) {
    double** res = mult_matrizConst(mult_constMatriz(matriz_DCT_INV, bloco->m,8), matriz_DCT_T_INV, 8);
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            res[i][j] += 128;
        }
    }
    return criarBloco(res, 0, 0, bloco->tipo);
}

BLOCO* aplicaQuantizacao(BLOCO* bloco){
    BLOCO* blocoQuantizado = criarBloco(bloco->m, 0, 0, bloco->tipo);
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

int* pega_zigzag(BLOCO* bloco) {
    int* vetor = (int*) malloc(sizeof(int)*64);
    int sobe = 1;
    int cnt = 0;

    // parte triangular superior
    for(int k = 0; k < 8; k++) {
        if (sobe) {
            // inicio: (k, 0)
            // fim: (0, k)
            for (int j = 0; j <= k; j++) {
                vetor[cnt] = (bloco->m)[k-j][0+j];
                cnt++;
            }
        } else {
            for (int j = 0; j <= k; j++) {
                vetor[cnt] = (bloco->m)[0+j][k-j];
                cnt++;
            }
        }
        sobe ^= 1;
    }

    // parte triangular inferior
    for(int k = 1; k < 8; k++) {
        if (sobe) {
            // inicio: (7, k)
            // fim: (k, 7)
            for (int j = 0; j < 8-k; j++) {
                vetor[cnt] = (bloco->m)[7-j][k+j];
                cnt++;
            }
        } else {
            for (int j = 0; j < 8-k; j++) {
                vetor[cnt] = (bloco->m)[k+j][7-j];
                cnt++;
            }
        }
        sobe ^= 1;
    }

    return vetor;
}
