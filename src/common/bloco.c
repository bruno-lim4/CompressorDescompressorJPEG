#include "bloco.h"
#include "util.h"
#include "consts.h"

// BLOCOS 8X8
struct bloco_ {
    double** m;
    char tipo; // L para luminancia, B para Cb, e R para CR
};

double getPosBloco(BLOCO *b, int i, int j){
    return (b->m)[i][j];
}

BLOCO* criarBloco(double** pos, int start_i, int start_j, char tipo) {
    // cria e aloca BLOCO 8x8
    BLOCO* bloco = (BLOCO*) malloc(sizeof(BLOCO));
        if (bloco == NULL) return NULL;

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
    desalocarMatriz_double(&((*bloco)->m), 8);
    free(*bloco);
        *bloco = NULL;
}

BLOCO* aplicaDCT(BLOCO* bloco);
BLOCO* desfazDCT(BLOCO* bloco);
BLOCO* aplicaQuantizacao(BLOCO* bloco);
int* pega_zigzag(BLOCO* bloco);

int* processaBloco(BLOCO* bloco) {
    BLOCO* bloco_dct = aplicaDCT(bloco);
    BLOCO* bloco_qtz = aplicaQuantizacao(bloco_dct);
    desalocarBloco(&bloco_dct);
    int* vetor = pega_zigzag(bloco_qtz);
    desalocarBloco(&bloco_qtz);
    return vetor;
}

// faz o level sampling tbm
BLOCO* aplicaDCT(BLOCO* bloco) {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            (bloco->m)[i][j] -= 128;
        }
    }

    double** DCT_m = mult_constMatriz(matriz_DCT, bloco->m, 8);
    double** DCT = mult_matrizConst(DCT_m, matriz_DCT_T, 8);

    desalocarMatriz_double(&DCT_m, 8);
    BLOCO* nbloco = criarBloco(DCT, 0, 0, bloco->tipo);
    desalocarMatriz_double(&DCT, 8);

    return nbloco;
}

// desfaz o level sampling tbm
BLOCO* desfazDCT(BLOCO* bloco) {
    double** DCT_INV_m = mult_constMatriz(matriz_DCT_T, bloco->m,8);
    double** res = mult_matrizConst(DCT_INV_m, matriz_DCT, 8);
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            res[i][j] += 128;
        }
    }

    BLOCO* nbloco = criarBloco(res, 0, 0, bloco->tipo);

    desalocarMatriz_double(&res, 8);
    desalocarMatriz_double(&DCT_INV_m, 8);

    return nbloco;
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

BLOCO* desfazQuantizacao(BLOCO* blocoQuantizado){
    BLOCO* bloco = criarBloco(blocoQuantizado->m, 0, 0, blocoQuantizado->tipo);
    int k = 1; //taxa de compressão.

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++){
            if(bloco->tipo == 'L')
                (bloco->m)[i][j] *= k*quant_luminancia[i][j];
            else
                (bloco->m)[i][j] *= k*quant_crominancia[i][j];
        }

    return bloco;
}

// Constrói o vetor DC + AC a partir de um bloco.
int* pega_zigzag(BLOCO* bloco) {
    int* vetor = (int*) malloc(sizeof(int)*64);
    if (vetor == NULL) return NULL;

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

// Constrói o bloco a partir de um vetor.
BLOCO* monta_bloco(int* vetor, char tipo) {
    BLOCO* bloco = (BLOCO*) malloc(sizeof(BLOCO));
    alocarMatriz_double(&(bloco->m), 8, 8);
    bloco->tipo = tipo;
    int sobe = 1;
    int cnt = 0;
    // parte triangular superior
    for(int k = 0; k < 8; k++) {
        if (sobe) {
            // inicio: (k, 0)
            // fim: (0, k)
            for (int j = 0; j <= k; j++) {
                (bloco->m)[k-j][0+j] = (double) vetor[cnt];
                cnt++;
            }
        } else {
            for (int j = 0; j <= k; j++) {
                (bloco->m)[0+j][k-j] = (double) vetor[cnt];
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
                (bloco->m)[7-j][k+j] = vetor[cnt];
                cnt++;
            }
        } else {
            for (int j = 0; j < 8-k; j++) {
                (bloco->m)[k+j][7-j] = vetor[cnt];
                cnt++;
            }
        }
        sobe ^= 1;
    }

    return bloco;
}

void gravaBloco(double **m, int start_i, int start_j, BLOCO *b){
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            m[start_i+i][start_j+j] = (b->m)[i][j];
}