#include "leitor.h"

struct leitor_{
    FILE* f;
    int buffer;
    int qtdBitsBuffer;
};

LEITOR* criarLeitor(FILE *f){
    LEITOR* l = malloc(sizeof(LEITOR));
    l->f = f;
    l->buffer = 0;
    l->qtdBitsBuffer = 0;

    return l;
}

void carregarBuffer(LEITOR* l){
    fread(&(l->buffer), 1, 1, l->f);
    l->qtdBitsBuffer = 8;
}
int lerBit(LEITOR *l){
    if(l->qtdBitsBuffer == 0){
        carregarBuffer(l);
    }

    int bit = (l->buffer >> l->qtdBitsBuffer-1) & 1;
    l->qtdBitsBuffer--;

    return bit;
}

int lerValor(int numBits, LEITOR* l){
    int valor = 0;
    int cont = numBits;
    while(cont--){
        valor <<= 1;
        valor |= lerBit(l);
    }

    if(!(valor & (1 << (numBits-1)))){ // Valor é negativo.
        int mascara = (1 << numBits) - 1;
        valor = (~valor) & mascara;

        return -valor;
    }
    
    return valor;
}