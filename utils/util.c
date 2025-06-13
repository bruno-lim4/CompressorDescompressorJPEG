#include "util.h"

void alocarMatriz_double(double*** m, int h, int w) {
    *m = (double **) malloc(h * sizeof(double*));
    if (*m == NULL) return;

    for(int i = 0; i < h; i++) {
        (*m)[i] = (double*) malloc(w*sizeof(double));
        if ((*m)[i] == NULL) return;
    }
}

void alocarMatriz_unsignedChar(unsigned char*** m, int h, int w) {
    *m = (unsigned char **) malloc(h * sizeof(unsigned char *));
    if (*m == NULL) return;

    for(int i = 0; i < h; i++) {
        (*m)[i] = (unsigned char *) malloc(w*sizeof(unsigned char));
        if ((*m)[i] == NULL) return;
    }
}

void desalocarMatriz_double(double ***m, int h , int w) {
    for(int i = 0; i < h; i++){
        free((*m)[i]);
        (*m)[i] = NULL;
    }

    free(*m);
    *m = NULL;
}

void desalocarMatriz_int(int ***m, int h , int w) {
    for(int i = 0; i < h; i++){
        free((*m)[i]);
        (*m)[i] = NULL;
    }

    free(*m);
    *m = NULL;
}

void desalocarMatriz_unsignedChar(unsigned char ***m, int h , int w) {
    for(int i = 0; i < h; i++){
        free((*m)[i]);
        (*m)[i] = NULL;
    }

    free(*m);
    *m = NULL;
}

double** mult_matrizQuadrada(double** a, double** b, int n) {
    double** res;
    alocarMatriz_double(&res, n, n);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            res[i][j] = 0;
            for(int k = 0; k < n; k++) {
                res[i][j] += a[i][k]*b[k][j];
            }
        }
    }

    return res;
}

double** mult_constMatriz(const double a[8][8], double** b, int n) {
    double** res;
    alocarMatriz_double(&res, n, n);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            res[i][j] = 0;
            for(int k = 0; k < n; k++) {
                res[i][j] += a[i][k]*b[k][j];
            }
        }
    }

    return res;
}

double** mult_matrizConst(double** a, const double b[8][8], int n) {
    double** res;
    alocarMatriz_double(&res, n, n);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            res[i][j] = 0;
            for(int k = 0; k < n; k++) {
                res[i][j] += a[i][k]*b[k][j];
            }
        }
    }

    return res;
}

double** matrizTransposta(double** a, int n) {
    double** res;
    alocarMatriz_double(&res, n, n);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            res[i][j] = a[j][i];
        }
    }

    return res; 
}

void printMatrizUchar(unsigned char** m, int h, int w){
    for(int i = 0; i < h; i++){
        for(int j = 0; j < w; j++){
            printf("[%d][%d]: %d\n", i, j, m[i][j]);
        }
        printf("\n");
    }
}

void printMatrizDouble(double** m, int h, int w){
    for(int i = 0; i < h; i++){
        for(int j = 0; j < w; j++){
            printf("[%d][%d]: %4.2lf\n", i, j, m[i][j]);
        }
        printf("\n");
    }
}

void print_binary(int value, int bits) {
    for(int i = bits-1; i >= 0; i--) {
        printf("%d", value&(1<<i) ? 1 : 0);
    }
    printf("\n");
}

int get_qtdBits(uint32_t value) {
    for(int i = 31; i >= 0; i--) {
        if ((1<<i)&value) return i+1;
    }
    return 0;
}

// coloca (a) (qtd_b) casas a esquerda e copia b nesse espaco
uint32_t shifta_e_grava(uint32_t a, uint32_t b, int qtd_b) {    
    a <<= qtd_b;
    for(int i = 0; i < qtd_b; i++) {
        a |= (1<<i)&b;
    }

    return a;
}

uint32_t get_mantissa_comp1(int value, int* qtd) {
    if (value > 0) {
        for(int i = 19; i >= 0; i--) {
            if ((1<<i)&value) {
                *qtd = i+1;
                return value;
            }
        }
    } else if (value < 0) {
        value = abs(value);
        for(int i = 19; i >= 0; i--) {
            if ((1<<i)&value) {
                *qtd = i+1;
                for(int k = i; k >= 0; k--) {
                    value ^= (1<<k);
                }
                return value;
            }
        }
    } else {
        *qtd = 0;
    }

    return 0;
}

