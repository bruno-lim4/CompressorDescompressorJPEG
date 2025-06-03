#include "util.h"

void alocarMatriz_double(double*** m, int h, int w) {
    *m = (double **) malloc(h * sizeof(double*));
    for(int i = 0; i < h; i++) {
        (*m)[i] = (double*) malloc(w*sizeof(double));
    }
}

void alocarMatriz_unsignedChar(unsigned char*** m, int h, int w) {
    *m = (unsigned char **) malloc(h * sizeof(unsigned char *));
    for(int i = 0; i < h; i++) {
        (*m)[i] = (unsigned char *) malloc(w*sizeof(unsigned char));
    }
}

void desalocarMatriz_double(double ***m, int h , int w) {
    for(int i = 0; i < h; i++){
        free((*m)[i]);
        (*m)[i] = NULL;
    }

    free(m);
    *m = NULL;
}

void desalocarMatriz_unsignedChar(unsigned char ***m, int h , int w) {
    for(int i = 0; i < h; i++){
        free((*m)[i]);
        (*m)[i] = NULL;
    }

    free(m);
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

