#include "imagem_comprimir.h"
#include "bloco.h"
#include "../compressor/gravador.h"

struct imagem_ {
    BMPFILEHEADER* fileHeader;
    BMPINFOHEADER* infoHeader;
    int h, w;
    int cbcr_h, cbcr_w;
    unsigned char **r, **g, **b;
    double **y, **cb, **cr;
};

void comprime_gravaMatriz(GRAVADOR* gravador, double** m, int i, int j, char tipoMatriz);

void comprimeImagem(IMAGEM* img, FILE* f) {
    // escreve, primeiramente, o cabecalho no .bin
    escreverFileHeader(f, img->fileHeader);
    escreverInfoHeader(f, img->infoHeader);

    // inicia o nosso gravador (buffer)
    GRAVADOR* gravador = criarGravador(f);

    // grava a matriz comprimida Y, depois Cb, e depois Cr
    comprime_gravaMatriz(gravador, img->y, img->h, img->w, 'L');
    comprime_gravaMatriz(gravador, img->cb, img->cbcr_h, img->cbcr_w, 'B');
    comprime_gravaMatriz(gravador, img->cr, img->cbcr_h, img->cbcr_w, 'R');

    // limpa o buffer
    finalizarGravacao(gravador);
    desalocarGravador(&gravador);
}

// pega o buffer (gravador) e grava a codificacao do vetor, tendo em vista o
// valor_difDC que é o valor da diferenca na posicao do DC
void codifica_gravaVetor(GRAVADOR* gravador, int* vetor, int valor_difDC) {
    // grava DC
    int qtd_DC;
    uint32_t codificaDC = codifica_infoDC(valor_difDC, &qtd_DC);
    gravarValor(gravador, codificaDC, qtd_DC);

    // varaivel que a guarda o tamanho da cadeia de zeros atual
    int qtd_zeros = 0;

    for(int i = 1; i < 64; i++) {
        if (vetor[i] == 0) {
            qtd_zeros++;
        } else {
            // se achei algum valor...
            int valor = vetor[i];

            // se tiver uma cadeia de mais, ou igual,a 15 bits
            while(qtd_zeros >= 15) {
                int qtd_15;
                uint32_t pref = codifica_infoAC(15, 0, &qtd_15);
                // grava o prefixo relacionado a esse evento
                gravarValor(gravador, pref, qtd_15);

                qtd_zeros -= 15;
            }

            // grava o par (qtd_zeros, valor)
            int qtd_AC;
            uint32_t codificaAC = codifica_infoAC(qtd_zeros, valor, &qtd_AC);
            gravarValor(gravador, codificaAC, qtd_AC);

            qtd_zeros = 0;
        }
    }


    // grava a sinalizacao de que acabou esse bloco
    int qtd_fim;
    uint32_t codifica_fim;

    codifica_fim = codifica_infoAC(0, 0, &qtd_fim);
    gravarValor(gravador, codifica_fim, qtd_fim);
}

void comprime_gravaMatriz(GRAVADOR* gravador, double** m, int h, int w, char tipoMatriz) {
    int ultimo_dc = 0; // guarda essa info pra codificacao por diferenca

    // loopa de bloco em bloco
    for(int i = 0; i < h; i += 8) {
        for(int j = 0; j < w; j += 8) {
            // cira um bloco
            BLOCO* bloco = criarBloco(m, i, j, tipoMatriz);
            // aplica DCT, quantizacao, e pega o vetor de 64 posicoes
            int* vetor_final = processaBloco(bloco);
            // codifica esse vetor e grava no meu .bin
            codifica_gravaVetor(gravador, vetor_final, vetor_final[0]-ultimo_dc);

            ultimo_dc = vetor_final[0];

            free(vetor_final); vetor_final = NULL;
            desalocarBloco(&bloco);
        }
    }
}