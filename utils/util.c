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

void print_binary(int value, int bits) {
    for(int i = bits-1; i >= 0; i--) {
        printf("%d", value&(1<<i) ? 1 : 0);
    }
    printf("\n");
}

uint32_t shifta_e_grava(uint32_t a, uint32_t b, int qtd_b) {    
    a <<= qtd_b;
    for(int i = 0; i < qtd_b; i++) {
        a |= (1<<i)&b;
    }

    return a;
}

uint32_t grava_infoDC(int value, int* qtd_bits) {
    int value_abs = abs(value);
    char cat = 'X';

    // pega a categoria
    if (value_abs == 0) cat = '0';
    else if (value_abs == 1) cat = '1';
    else if (value_abs <= 3) cat = '2';
    else if (value_abs <= 7) cat = '3';
    else if (value_abs <= 15) cat = '4';
    else if (value_abs <= 31) cat = '5';
    else if (value_abs <= 63) cat = '6';
    else if (value_abs <= 127) cat = '7';
    else if (value_abs <= 255) cat = '8';
    else if (value_abs <= 511) cat = '9';
    else if (value_abs <= 1023) cat = 'A';

    uint32_t res = 0;

    // guarda o prefixo
    switch (cat)
    {
    case '0':
        res = shifta_e_grava(res, 2, 3);
        break;
    
    case '1':
        res = shifta_e_grava(res, 3, 3);
        break;
    
    case '2':
        res = shifta_e_grava(res, 4, 3);
        break;
    
    case '3':
        res = shifta_e_grava(res, 0, 2);
        break;
    
    case '4':
        res = shifta_e_grava(res, 5, 3);
        break;

    case '5':
        res = shifta_e_grava(res, 6, 3);
        break;
    
    case '6':
        res = shifta_e_grava(res, 14, 4);
        break;
    
    case '7':
        res = shifta_e_grava(res, 30, 5);
        break;
    
    case '8':
        res = shifta_e_grava(res, 62, 6);
        break;
    
    case '9':
        res = shifta_e_grava(res, 126, 7);
        break;
    
    case 'A':
        res = shifta_e_grava(res, 254, 8);
        break;
    
    default:
        break;
    }

    // guarda a mantissa em complemento de 1

}