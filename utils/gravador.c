#include "gravador.h"
#include "util.h"

struct gravador_ {
    FILE* arquivo;
    uint32_t mascara;
    int qtd_atual;
};

void salvaMascara(GRAVADOR* gravador);

GRAVADOR* criarGravador(FILE* f) {
    GRAVADOR* gravador = (GRAVADOR*) malloc(sizeof(GRAVADOR));
    gravador->mascara = 0;
    gravador->qtd_atual = 0;
    gravador->arquivo = f;

    return gravador;
}

void gravarValor(GRAVADOR* gravador, uint32_t value, int qtd_bits) {
    if (gravador->qtd_atual + qtd_bits > 32) {
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

    gravador->mascara = shifta_e_grava(gravador->mascara, value, qtd_bits);
    gravador->qtd_atual += qtd_bits;
}

// ao inves de gravar direto, talvez salvar cada mascara em um array na memoria e dps gravar tudo
void salvaMascara(GRAVADOR* gravador) {
    if (gravador->qtd_atual == 32) {
        // grava tudo
        fwrite(&(gravador->mascara), sizeof(uint32_t), 1, gravador->arquivo);
    } else {
        gravador->mascara <<= (32-gravador->qtd_atual);
        fwrite(&(gravador->mascara), sizeof(uint32_t), 1, gravador->arquivo);
    }

    gravador->mascara = 0;
    gravador->qtd_atual = 0;
}