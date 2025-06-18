#include "leitor.h"

// Estrutura do leitor.
struct leitor_{
    FILE* f;
    uint32_t buffer;
    int qtdBitsBuffer;
};

LEITOR* criarLeitor(FILE *f){
    LEITOR* l = malloc(sizeof(LEITOR));
    l->f = f;
    l->buffer = 0;
    l->qtdBitsBuffer = 0;

    return l;
}

void carregarBuffer(LEITOR* l){ // Lê bytes do arquivo e guarda em l->buffer.
    int numBytesLidos = fread(&(l->buffer), 1, 4, l->f); // Tenta ler 4 bytes do arquivo.
    l->qtdBitsBuffer = 8*numBytesLidos; // Se estiver no fim do arquivo, pode não ter 4 bytes para ler.
}

int lerBit(LEITOR *l){ // Retorna o bit menos significativo do buffer.
    if(l->qtdBitsBuffer == 0){ // Verifica se o buffer está vazio.
        carregarBuffer(l);
    }

    int bit = (l->buffer >> ((l->qtdBitsBuffer)-1)) & 1; // Pega o bit menos significativo do buffer.
    l->qtdBitsBuffer--;

    return bit;
}

int lerValor(int numBits, LEITOR* l){ // Lê um valor (positivo ou negativo) de comprimento 'numBits' do arquivo.
    int valor = 0;
    int cont = numBits;
    while(cont--){ // Lê um bit por vez e o armazena no bit menos significativo de 'valor'.
        valor <<= 1;
        valor |= lerBit(l);
    }

    if(!(valor & (1 << (numBits-1)))){ // Se o bit mais significativo for 0, o valor é negativo.
        int mascara = (1 << numBits) - 1;
        valor = (~valor) & mascara; // Inverte todos os bits (complemento de 1).

        return -valor;
    }
    
    return valor;
}

void destruirLeitor(LEITOR** l){ // Libera a memória.
    free(*l);
    l = NULL;
}