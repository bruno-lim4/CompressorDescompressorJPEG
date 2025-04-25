#include "imagem.h"

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

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
            

            (img->y)[i][j] = 0.299*(double)(img->r)[i][j] + 0.587*(double)(img->g)[i][j] + 0.114*(double)(img->b)[i][j];
        }
    }

    alocarMatriz_double(&(img->cb), (img->h)/2, (img->w)/2);
    alocarMatriz_double(&(img->cr), (img->h)/2, (img->w)/2);

    int new_i = 0;
    double most_negative = 0.0;
    double grande = 0.0;

    for(int i = 0; i < (img->h); i+=2) {
        int new_j = 0;
        for(int j = 0; j < (img->w); j+=2) {
            // Cb = (S(0.564(Bi − Yi) + 128)) / 4
            double soma_bloco_cb = ((double)(img->b)[i][j]-(img->y)[i][j]) + ((double)(img->b)[i+1][j]-(img->y)[i+1][j]) + ((double)(img->b)[i+1][j+1]-(img->y)[i+1][j+1]) + ((double)(img->b)[i][j+1]-(img->y)[i][j+1]);
            double soma_bloco_cr = ((double)(img->r)[i][j]-(img->y)[i][j]) + ((double)(img->r)[i+1][j]-(img->y)[i+1][j]) + ((double)(img->r)[i+1][j+1]-(img->y)[i+1][j+1]) + ((double)(img->r)[i][j+1]-(img->y)[i][j+1]);

            (img->cb)[new_i][new_j] = ((0.564*soma_bloco_cb)/4.0) + 128.0;
            (img->cr)[new_i][new_j] = ((0.713*soma_bloco_cr)/4.0) + 128.0;

            /*
            if ((img->cb)[new_i][new_j] < most_negative) {
                most_negative = (img->cb)[new_i][new_j];
            }
            if ((img->cr)[new_i][new_j] < most_negative) {
                most_negative = (img->cr)[new_i][new_j];
            }

            if ((img->cb)[new_i][new_j] > grande) {
                grande = (img->cb)[new_i][new_j];
            }
            if ((img->cr)[new_i][new_j] > grande) {
                grande = (img->cr)[new_i][new_j];
            }
            */

            new_j++;
        }
        new_i++;
    }

    //printf("%lf / %lf\n", most_negative, grande);

    return img;
}



void escreverImagem(FILE* f, IMAGEM* img) {
    escreverFileHeader(f, img->fileHeader);
    escreverInfoHeader(f, img->infoHeader);

    double menor = 0.0;
    double maior = 0.0;

    for(int i = 0; i < img->h; i++) {
        for(int j = 0; j < img->w; j++) {
            double cr = (img->cr)[i/2][j/2] - 128.0;
            double cb = (img->cb)[i/2][j/2] - 128.0;

            double r_ = (img->y)[i][j] + 1.402*cr;
            double g_ = (img->y)[i][j] - 0.344*cb - 0.714*cr;
            double b_ = (img->y)[i][j] + 1.772*cb;

            unsigned char r = (unsigned char) max(0, min(255, r_));
            unsigned char g = (unsigned char) max(0, min(255, g_));
            unsigned char b = (unsigned char) max(0, min(255, b_));

            /*
            if (r_ > maior) {
                maior = r_;
            }
            if (g_ > maior) {
                maior = g_;
            }
            if (b_ > maior) {
                maior = b_;
            }
            if (r_ < menor) {
                menor = r_;
            }
            if (g_ < menor) {
                menor = g_;
            }
            if (b_ < menor) {
                menor = b_;
            }
            */

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

    //printf("%lf / %lf\n", menor, maior);
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