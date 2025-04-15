#include "util.h"

void alocarMatriz_double(double*** m, int h, int w) {
    *m = (double **) malloc(h * sizeof(double*));
    for(int i = 0; i < h; i++) {
        (*m)[i] = (double*) malloc(w*sizeof(double));
    }
}

void alocarMatriz_unsignedChar(unsigned char *** m, int h, int w) {
    *m = (unsigned char **) malloc(h * sizeof(unsigned char *));
    for(int i = 0; i < h; i++) {
        (*m)[i] = (unsigned char *) malloc(w*sizeof(unsigned char));
    }
}