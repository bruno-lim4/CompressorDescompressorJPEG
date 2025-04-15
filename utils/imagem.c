#include "imagem.h"

struct imagem_ {
    BMPFILEHEADER* fileHeader;
    BMPINFOHEADER* infoHeader;
    int h, w;
    unsigned char **r, **g, **b;
    double **y, **cb, **cr;
};

IMAGEM* criarImagem(FILE* f) {
    IMAGEM* img = (IMAGEM*) malloc(sizeof(IMAGEM));
    img->fileHeader = leituraFileHeader(f);
    img->infoHeader = leituraInfoHeader(f);

    img->w = get_biWidth(img->infoHeader);
    img->h = get_biHeight(img->infoHeader);

    int qtd_pixels = img->w*img->h;

    alocarMatriz_unsignedChar(&(img->r), img->h, img->w);
    alocarMatriz_unsignedChar(&(img->g), img->h, img->w);
    alocarMatriz_unsignedChar(&(img->b), img->h, img->w);

    alocarMatriz_double(&(img->y), img->h, img->w);

    for(int i = 0; i < img->h; i++) {
        for(int j = 0; j < img->w; j++) {
            fread(&((img->b)[i][j]), sizeof(unsigned char), 1, f);
            fread(&((img->g)[i][j]), sizeof(unsigned char), 1, f);
            fread(&((img->r)[i][j]), sizeof(unsigned char), 1, f);

            /*
            (img->r)[i][j] -= 128;
            (img->g)[i][j] -= 128;
            (img->b)[i][j] -= 128;
            */
            

            (img->y)[i][j] = 0.299*(img->r)[i][j] + 0.587*(img->g)[i][j] + 0.114*(img->b)[i][j];
        }
    }

    alocarMatriz_double(&(img->cb), (img->h)/2, (img->w)/2);
    alocarMatriz_double(&(img->cr), (img->h)/2, (img->w)/2);

    int new_i = 0;

    for(int i = 0; i < (img->h); i+=2) {
        int new_j = 0;
        for(int j = 0; j < (img->w); j+=2) {
            // Cb = 0.564(B − Y ) 
            double soma_bloco_cb = ((img->b)[i][j]-(img->y)[i][j]) + ((img->b)[i+1][j]-(img->y)[i+1][j]) + ((img->b)[i+1][j+1]-(img->y)[i+1][j+1]) + ((img->b)[i][j+1]-(img->y)[i][j+1]);
            double soma_bloco_cr = ((img->r)[i][j]-(img->y)[i][j]) + ((img->r)[i+1][j]-(img->y)[i+1][j]) + ((img->r)[i+1][j+1]-(img->y)[i+1][j+1]) + ((img->r)[i][j+1]-(img->y)[i][j+1]);

            (img->cb)[new_i][new_j] = (0.564*soma_bloco_cb)/4.0;
            (img->cr)[new_i][new_j] = (0.713*soma_bloco_cr)/4.0;
            new_j++;
        }
        new_i++;
    }

    return img;
}

void escreverImagem(FILE* f, IMAGEM* img) {
    escreverFileHeader(f, img->fileHeader);
    escreverInfoHeader(f, img->infoHeader);

    for(int i = 0; i < img->h; i++) {
        for(int j = 0; j < img->w; j++) {
            unsigned char r = (img->y)[i][j] + 1.402*(img->cr)[i/2][j/2];
            unsigned char g = (img->y)[i][j] - 0.344*(img->cb)[i/2][j/2] - 0.714*(img->cr)[i/2][j/2];
            unsigned char b = (img->y)[i][j] + 1.772*(img->cb)[i/2][j/2];
            /*
            r+=128;
            g+=128;
            b+=128;
            */
            fwrite(&b, sizeof(unsigned char), 1, f);
            fwrite(&g, sizeof(unsigned char), 1, f);
            fwrite(&r, sizeof(unsigned char), 1, f);
        }
    }
}

void printarImagem(IMAGEM* img) {
    printFileHeader(img->fileHeader);
    printInfoHeader(img->infoHeader);
    /*
    for(int i = 0; i < img->h/2; i++) {
        for(int j = 0; j < img->w/2; j++) {
            printf("(%lf %lf %lf)(%d, %d), ", (img->y)[i][j], (img->cb)[i][j], (img->cr)[i][j], i, j);
        }
    }
    */
    return;
}