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
    escreverFileHeader(f, img->fileHeader);
    escreverInfoHeader(f, img->infoHeader);

    GRAVADOR* gravador = criarGravador(f);

    comprime_gravaMatriz(gravador, img->y, img->h, img->w, 'L');
    comprime_gravaMatriz(gravador, img->cb, img->cbcr_h, img->cbcr_w, 'B');
    comprime_gravaMatriz(gravador, img->cr, img->cbcr_h, img->cbcr_w, 'R');

    finalizarGravacao(gravador);
    desalocarGravador(&gravador);
}

void codifica_gravaVetor(GRAVADOR* gravador, int* vetor, int valor_difDC) {
    // grava DC
    int qtd_DC;
    uint32_t codificaDC = codifica_infoDC(valor_difDC, &qtd_DC);
    gravarValor(gravador, codificaDC, qtd_DC);

    int qtd_zeros = 0;

    for(int i = 1; i < 64; i++) {
        if (vetor[i] == 0) {
            qtd_zeros++;
        } else {
            int valor = vetor[i];

            while(qtd_zeros >= 15) {
                int qtd_15;
                uint32_t pref = codifica_infoAC(15, 0, &qtd_15);
                gravarValor(gravador, pref, qtd_15);

                qtd_zeros -= 15;
            }

            int qtd_AC;
            uint32_t codificaAC = codifica_infoAC(qtd_zeros, valor, &qtd_AC);
            gravarValor(gravador, codificaAC, qtd_AC);

            qtd_zeros = 0;
        }
    }

    int qtd_fim;
    uint32_t codifica_fim;

    codifica_fim = codifica_infoAC(0, 0, &qtd_fim);
    gravarValor(gravador, codifica_fim, qtd_fim);
}

void comprime_gravaMatriz(GRAVADOR* gravador, double** m, int h, int w, char tipoMatriz) {
    int ultimo_dc = 0;

    for(int i = 0; i < h; i += 8) {
        for(int j = 0; j < w; j += 8) {
            BLOCO* bloco = criarBloco(m, i, j, tipoMatriz);
            int* vetor_final = processaBloco(bloco);
            codifica_gravaVetor(gravador, vetor_final, vetor_final[0]-ultimo_dc);
            ultimo_dc = vetor_final[0];

            free(vetor_final); vetor_final = NULL;
            desalocarBloco(&bloco);
        }
    }
}