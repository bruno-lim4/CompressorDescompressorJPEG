#include "leitor.h"
#include "imagem.h"

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
    int numBytesLidos = fread(&(l->buffer), 1, 4, l->f); // Tenta ler 4 bytes do arquivo.
    l->qtdBitsBuffer = 8*numBytesLidos; // Se estiver no fim do arquivo, pode não ter 4 bytes para ler.
    printf("numBytesLidos: %d tamBuffer: %d\n\n", numBytesLidos, l->qtdBitsBuffer);
}

int lerBit(LEITOR *l){
    if(l->qtdBitsBuffer == 0){
        carregarBuffer(l);
    }

    // Debug.
    //printf("\nna lerBit()\nbuffer:\n\n");
    //printarInt(l->buffer);

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