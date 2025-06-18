#include "gravador.h"
#include "util.h"

struct gravador_ {
    FILE* arquivo; // arquivo .bin
    uint32_t mascara; // mascara (buffer) de 4 bytes
    int qtd_atual; // qtd de bits que representam um valor valido nesse buffer
};

void salvaMascara(GRAVADOR* gravador);

GRAVADOR* criarGravador(FILE* f) {
    GRAVADOR* gravador = (GRAVADOR*) malloc(sizeof(GRAVADOR));
    if (gravador == NULL) return NULL;

    gravador->mascara = 0;
    gravador->qtd_atual = 0;
    gravador->arquivo = f;

    return gravador;
}

void gravarValor(GRAVADOR* gravador, uint32_t value, int qtd_bits) {
    if (gravador->qtd_atual + qtd_bits > 32) { // se nao cabe todo o valor no meu buffer
        int tam1 = 32 - gravador->qtd_atual;
        int tam2 = qtd_bits-tam1;

        // value = [gravar agora - value1][gravar depois - value2]

        // pega os tam1 bits mais significativos de value
        uint32_t value1 = 0;
        for(int i = 0; i < tam1; i++) {
            value1 |= (1<<i)&(value>>tam2);
        }

        // pega os tam2 bits menos significativos de value
        uint32_t value2 = 0;
        for(int i = 0; i < tam2; i++) {
            value2 |= (1<<i)&(value);
        }

        gravador->mascara = shifta_e_grava(gravador->mascara, value1, tam1);
        gravador->qtd_atual = 32;
        salvaMascara(gravador);
        
        gravador->mascara = value2;
        gravador->qtd_atual = tam2;

        return;
    } 

    // cabe no meu buffer
    gravador->mascara = shifta_e_grava(gravador->mascara, value, qtd_bits);
    gravador->qtd_atual += qtd_bits;
}

void finalizarGravacao(GRAVADOR* gravador) {
    if (gravador->qtd_atual > 0) salvaMascara(gravador);
}

// funcao interna que grava o buffer no arquivo
void salvaMascara(GRAVADOR* gravador) {
    if (gravador->qtd_atual == 32) {
        // grava tudo
        fwrite(&(gravador->mascara), sizeof(uint32_t), 1, gravador->arquivo);
    } else {
        // nao fechei meu buffer, entao shifto geral pra esquerda e gravo
        gravador->mascara <<= (32-gravador->qtd_atual);
        fwrite(&(gravador->mascara), sizeof(uint32_t), 1, gravador->arquivo);
    }

    gravador->mascara = 0;
    gravador->qtd_atual = 0;
}

void desalocarGravador(GRAVADOR** gravador) {
    free(*gravador);
    *gravador = NULL;
}