#include "imagem.h"
#include "bloco.h"
#include "../compressor/codifica.h"
#include "../compressor/gravador.h"
#include "../descompressor/decodifica.h"
#include "../descompressor/leitor.h"

struct imagem_ {
    BMPFILEHEADER* fileHeader;
    BMPINFOHEADER* infoHeader;
    int h, w;
    int cbcr_h, cbcr_w;
    unsigned char **r, **g, **b;
    double **y, **cb, **cr;
};

IMAGEM* criarImagem(FILE* f) {
    // cria imagem
    IMAGEM* img = (IMAGEM*) malloc(sizeof(IMAGEM));
    if (img == NULL) return NULL;

    // pega o cabecalho
    img->fileHeader = leituraFileHeader(f);
    img->infoHeader = leituraInfoHeader(f);

    printFileHeader(img->fileHeader);
    printInfoHeader(img->infoHeader);

    // pega as dimensoes no cabecalho
    img->w = get_biWidth(img->infoHeader);
    img->h = get_biHeight(img->infoHeader);

    // aloca matrizes r,g,b
    alocarMatriz_unsignedChar(&(img->r), img->h, img->w);
    alocarMatriz_unsignedChar(&(img->g), img->h, img->w);
    alocarMatriz_unsignedChar(&(img->b), img->h, img->w);

    // aloca luminancia
    alocarMatriz_double(&(img->y), img->h, img->w);

    // le r,g,b e ja calcula luminancia
    for(int i = 0; i < img->h; i++) {
        for(int j = 0; j < img->w; j++) {
            if (fread(&img->b[i][j], sizeof(unsigned char), 1, f) != 1 ||
                fread(&img->g[i][j], sizeof(unsigned char), 1, f) != 1 ||
                fread(&img->r[i][j], sizeof(unsigned char), 1, f) != 1) {
                fprintf(stderr, "Erro: fread falhou na posição (%d, %d)\n", i, j);
                exit(EXIT_FAILURE);
            }

            (img->y)[i][j] = 0.299*(double)(img->r)[i][j] + 0.587*(double)(img->g)[i][j] + 0.114*(double)(img->b)[i][j];
        }
    }


    // calcula as dimensoes que cb,cr vao ter
    // e "corrige" para ser multiplo de 8
    img->cbcr_w = (img->w)/2 + ( (((img->w)/2)%8) ? (8 - ((img->w)/2)%8) : 0);
    img->cbcr_h = (img->h)/2 + ( (((img->h)/2)%8) ? (8 - ((img->h)/2)%8) : 0);

    printf("cbcrNovo = (%d, %d)\n", img->cbcr_h, img->cbcr_w);
    // aloca essas matrizes
    alocarMatriz_double(&(img->cb), img->cbcr_h, img->cbcr_w);
    alocarMatriz_double(&(img->cr), img->cbcr_h, img->cbcr_w);

    int new_i = 0;

    // calcula cb,cr (faz o downsampling)
    for(int i = 0; i < (img->h); i+=2) {
        int new_j = 0;
        for(int j = 0; j < (img->w); j+=2) {
            double soma_bloco_cb = ((double)(img->b)[i][j]-(img->y)[i][j]) + ((double)(img->b)[i+1][j]-(img->y)[i+1][j]) + ((double)(img->b)[i+1][j+1]-(img->y)[i+1][j+1]) + ((double)(img->b)[i][j+1]-(img->y)[i][j+1]);
            double soma_bloco_cr = ((double)(img->r)[i][j]-(img->y)[i][j]) + ((double)(img->r)[i+1][j]-(img->y)[i+1][j]) + ((double)(img->r)[i+1][j+1]-(img->y)[i+1][j+1]) + ((double)(img->r)[i][j+1]-(img->y)[i][j+1]);

            (img->cb)[new_i][new_j] = ((0.564*soma_bloco_cb)/4.0) + 128.0;
            (img->cr)[new_i][new_j] = ((0.713*soma_bloco_cr)/4.0) + 128.0;

            new_j++;
        }
        new_i++;
    }

    // agora copia linhas/coluna para preencher 
    // o espaço que falta (pq precisa ser mult de 8)

    int old_w = (img->w)/2;
    int old_h = (img->h)/2;

    // conserta coluna
    for(int k = 0; k < old_h; k++) {
        double value_cb = (img->cb)[k][old_w-1];
        double value_cr = (img->cr)[k][old_w-1];

        for(int j = old_w; j < img->cbcr_w; j++) {
            (img->cb)[k][j] = value_cb;
            (img->cr)[k][j] = value_cr;
        }
    }

    // conserta linha
    for(int k = 0; k < img->cbcr_w; k++){
        double value_cb = (img->cb)[old_h-1][k];
        double value_cr = (img->cr)[old_h-1][k];

        for(int j = old_h; j < img->cbcr_h; j++){
            (img->cb)[j][k] = value_cb;
            (img->cr)[j][k] = value_cr;
        }
    }

    // antes de retornar, coloca todos os valores no intervalo [0, 255]
    for(int i = 0; i < img->h; i++) {
        for(int j = 0; j < img->w; j++) {
            if (i < img->cbcr_h && j < img->cbcr_w) {
                img->cb[i][j] =  max(0, min(255, img->cb[i][j]));
                img->cr[i][j] =  max(0, min(255, img->cr[i][j]));
            }
            img->y[i][j] =  max(0, min(255, img->y[i][j]));
        }
    }

    return img;
}


// Salva a imagem no arquivo.
void salvarImagem(FILE* f, IMAGEM* img) {
    // Escreve cabeçalhos
    fseek(f, 0, SEEK_SET);
    escreverFileHeader(f, img->fileHeader);
    escreverInfoHeader(f, img->infoHeader);

    int largura = img->w;
    int altura = img->h;

    for (int i = 0; i < altura; i++){
        for (int j = 0; j < largura; j++) {
            fwrite(&img->b[i][j], 1, 1, f); // Azul
            fwrite(&img->g[i][j], 1, 1, f); // Verde
            fwrite(&img->r[i][j], 1, 1, f); // Vermelho
        }
    }
}

void desalocarImagem(IMAGEM** img) {
    desalocarFileHeader(&((*img)->fileHeader));
    desalocarInfoHeader(&((*img)->infoHeader));

    desalocarMatriz_unsignedChar(&((*img)->r), (*img)->h);
    desalocarMatriz_unsignedChar(&((*img)->g), (*img)->h);
    desalocarMatriz_unsignedChar(&((*img)->b), (*img)->h);

    desalocarMatriz_double(&((*img)->y), (*img)->h);
    desalocarMatriz_double(&((*img)->cb), (*img)->cbcr_h);
    desalocarMatriz_double(&((*img)->cr), (*img)->cbcr_h);

    free(*img);

    *img = NULL; 
}