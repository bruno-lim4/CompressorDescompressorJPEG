#include "header.h"

struct bmpfileheader_ {
    unsigned short bfType; /* Magic number for file */
    unsigned int bfSize; /* Size of file */
    unsigned short bfReserved1; /* Reserved */
    unsigned short bfReserved2; /* ... */
    unsigned int bfOffBits; /* Offset to bitmap data */
};

struct bmpinfoheader_ {
    unsigned int biSize; /* Size of info header */
    int biWidth; /* Width of image */
    int biHeight; /* Height of image */
    unsigned short biPlanes; /* Number of color planes */
    unsigned short biBitCount; /* Number of bits per pixel */
    unsigned int biCompression; /* Type of compression to use */
    unsigned int biSizeImage; /* Size of image data */
    int biXPelsPerMeter; /* X pixels per meter */
    int biYPelsPerMeter; /* Y pixels per meter */
    unsigned int biClrUsed; /* Number of colors used */
    unsigned int biClrImportant; /* Number of important colors */
};


BMPFILEHEADER* criarFileHeader(unsigned short bfType, unsigned int bfSize, unsigned short bfReserved1, unsigned short bfReserved2, unsigned int bfOffBits) {
    BMPFILEHEADER* fileHeader = (BMPFILEHEADER*) malloc(sizeof(BMPFILEHEADER));
    if (fileHeader == NULL) return NULL;

    fileHeader->bfType = bfType;
    fileHeader->bfSize = bfSize;
    fileHeader->bfReserved1 = bfReserved1;
    fileHeader->bfReserved2 = bfReserved2;
    fileHeader->bfOffBits = bfOffBits;


    return fileHeader;
}

BMPFILEHEADER* leituraFileHeader(FILE* arq) {
    unsigned short bfType, bfReserved1, bfReserved2;
    unsigned int bfSize, bfOffBits;

    fread(&bfType, sizeof(unsigned short int), 1, arq);
    fread(&bfSize, sizeof(unsigned int), 1, arq);
    fread(&bfReserved1, sizeof(unsigned short int), 1, arq);
    fread(&bfReserved2, sizeof(unsigned short int), 1, arq);
    fread(&bfOffBits, sizeof(unsigned int), 1, arq);

    return criarFileHeader(bfType, bfSize, bfReserved1, bfReserved2, bfOffBits);
}

void escreverFileHeader(FILE* arq, BMPFILEHEADER* fileHeader) {
    fwrite(&fileHeader->bfType, sizeof(unsigned short int), 1, arq);
    fwrite(&fileHeader->bfSize, sizeof(unsigned int), 1, arq);
    fwrite(&fileHeader->bfReserved1, sizeof(unsigned short int), 1, arq);
    fwrite(&fileHeader->bfReserved2, sizeof(unsigned short int), 1, arq);
    fwrite(&fileHeader->bfOffBits, sizeof(unsigned int), 1, arq);
}

void desalocarFileHeader(BMPFILEHEADER** fileHeader) {
    free(*fileHeader);
    *fileHeader = NULL;
}

void printFileHeader(BMPFILEHEADER* fileHeader) {
    printf("bfType: %hu\nbfSize: %hu\nbfReserved1: %hu\nbfReserved2: %hu\nbfOffBits: %hu\n", fileHeader->bfType, fileHeader->bfSize, fileHeader->bfReserved1, fileHeader->bfReserved2, fileHeader->bfOffBits);
}

BMPINFOHEADER* criarInfoHeader(unsigned int biSize, int biWidth, int biHeight, unsigned short biPlanes, unsigned short biBitCount, unsigned int biCompression, unsigned int biSizeImage, int biXPelsPerMeter, int biYPelsPerMeter, unsigned int biClrUsed, unsigned int biClrImportant) {
    BMPINFOHEADER* infoHeader = (BMPINFOHEADER*) malloc(sizeof(BMPINFOHEADER));
    if (infoHeader == NULL) return NULL;

    infoHeader->biSize = biSize;
    infoHeader->biWidth = biWidth;
    infoHeader->biHeight = biHeight;
    infoHeader->biPlanes = biPlanes;
    infoHeader->biBitCount = biBitCount;
    infoHeader->biCompression = biCompression;
    infoHeader->biSizeImage = biSizeImage;
    infoHeader->biXPelsPerMeter = biXPelsPerMeter;
    infoHeader->biYPelsPerMeter = biYPelsPerMeter;
    infoHeader->biClrUsed = biClrUsed;
    infoHeader->biClrImportant = biClrImportant;

    return infoHeader;
}

BMPINFOHEADER* leituraInfoHeader(FILE* arq) {
    unsigned int biSize;
    int biWidth, biHeight, biXPelsPerMeter, biYPelsPerMeter;
    unsigned short biPlanes, biBitCount;
    unsigned int biClrUsed, biClrImportant, biCompression, biSizeImage; 

    fread(&biSize, sizeof(unsigned int), 1, arq);
    fread(&biWidth, sizeof(int), 1, arq);
    fread(&biHeight, sizeof(int), 1, arq);
    fread(&biPlanes, sizeof(unsigned short), 1, arq);
    fread(&biBitCount, sizeof(unsigned short), 1, arq);
    fread(&biCompression, sizeof(unsigned int), 1, arq);
    fread(&biSizeImage, sizeof(unsigned int), 1, arq);
    fread(&biXPelsPerMeter, sizeof(int), 1, arq);
    fread(&biYPelsPerMeter, sizeof(int), 1, arq);
    fread(&biClrUsed, sizeof(unsigned int), 1, arq);
    fread(&biClrImportant, sizeof(unsigned int), 1, arq);
    
    return criarInfoHeader(biSize, biWidth, biHeight, biPlanes, biBitCount, biCompression, biSizeImage, biXPelsPerMeter, biYPelsPerMeter, biClrUsed, biClrImportant);
}

void escreverInfoHeader(FILE* arq, BMPINFOHEADER* infoHeader) {
    fwrite(&infoHeader->biSize, sizeof(unsigned int), 1, arq);
    fwrite(&infoHeader->biWidth, sizeof(int), 1, arq);
    fwrite(&infoHeader->biHeight, sizeof(int), 1, arq);
    fwrite(&infoHeader->biPlanes, sizeof(unsigned short), 1, arq);
    fwrite(&infoHeader->biBitCount, sizeof(unsigned short), 1, arq);
    fwrite(&infoHeader->biCompression, sizeof(unsigned int), 1, arq);
    fwrite(&infoHeader->biSizeImage, sizeof(unsigned int), 1, arq);
    fwrite(&infoHeader->biXPelsPerMeter, sizeof(int), 1, arq);
    fwrite(&infoHeader->biYPelsPerMeter, sizeof(int), 1, arq);
    fwrite(&infoHeader->biClrUsed, sizeof(unsigned int), 1, arq);
    fwrite(&infoHeader->biClrImportant, sizeof(unsigned int), 1, arq);
}

int get_biWidth(BMPINFOHEADER* infoHeader) {
    return infoHeader->biWidth;
}
int get_biHeight(BMPINFOHEADER* infoHeader) {
    return infoHeader->biHeight;
}

void desalocarInfoHeader(BMPINFOHEADER** infoHeader) {
    free(*infoHeader);
    *infoHeader = NULL;
    return;
}

void printInfoHeader(BMPINFOHEADER* infoHeader) {
    printf("biSize: %hu\nbiWidth: %d\nbiHeight: %d\nbiPlanes: %hu\nbiBitCount: %hu\nbiCompression: %hu\nbiSizeImage: %hu\nbiXPelsPerMeter: %d\nbiYPelsPerMeter: %d\nbiClrUsed: %hu\nbiClrImportant: %hu\n", infoHeader->biSize, infoHeader->biWidth, infoHeader->biHeight, infoHeader->biPlanes, infoHeader->biBitCount, infoHeader->biCompression, infoHeader->biSizeImage, infoHeader->biXPelsPerMeter, infoHeader->biYPelsPerMeter, infoHeader->biClrUsed, infoHeader->biClrImportant);
}

int checaInfoFileHeader(BMPFILEHEADER* fileHeader, BMPINFOHEADER* infoHeader) {
    if (fileHeader->bfOffBits != 54) {
        desalocarFileHeader(&fileHeader);
        desalocarInfoHeader(&infoHeader);
        fprintf(stderr, "ERRO: imagem tem tamanho de cabeçalho inválido. Deve ser de 54 bytes.");
        return 0;
    }
    if (infoHeader->biBitCount != 24) {
        desalocarFileHeader(&fileHeader);
        desalocarInfoHeader(&infoHeader);
        fprintf(stderr, "ERRO: imagem tem quantidade de bits por pixel inválida. Deve ser de 24 bits.");
        return 0;
    }
    if (infoHeader->biWidth % 8 != 0 || infoHeader->biHeight % 8 != 0) {
        desalocarFileHeader(&fileHeader);
        desalocarInfoHeader(&infoHeader);
        fprintf(stderr, "ERRO: imagem tem que ter largura e altura múltiplas de 8.");
        return 0;
    }

    return 1;
}
