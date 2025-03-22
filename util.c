#include "util.h"

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

struct pixel_ {
    char r, g, b;
};

PIXEL* criarPixel(char r, char g, char b) {
    PIXEL* pixel = (PIXEL*) malloc(sizeof(PIXEL));
    pixel->r = r;
    pixel->g = g;
    pixel->b = b;
    return pixel;
}

PIXEL* leituraPixel(FILE* f) {
    char r, g, b;
    fread(&b, sizeof(char), 1, f);
    fread(&g, sizeof(char), 1, f);
    fread(&r, sizeof(char), 1, f);

    PIXEL* pixel = criarPixel(r, g, b);
    return pixel;
}

void printPixel(PIXEL* pixel) {
    printf("(%hhx, %hhx, %hhx)", pixel->r, pixel->g, pixel->b);
}

BMPFILEHEADER* criarFileHeader(unsigned short bfType, unsigned int bfSize, unsigned short bfReserved1, unsigned short bfReserved2, unsigned int bfOffBits) {
    BMPFILEHEADER* fileHeader = (BMPFILEHEADER*) malloc(sizeof(BMPFILEHEADER));
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

void printFileHeader(BMPFILEHEADER* fileHeader) {
    printf("bfType: %hu\nbfSize: %hu\nbfReserved1: %hu\nbfReserved2: %hu\nbfOffBits: %hu\n", fileHeader->bfType, fileHeader->bfSize, fileHeader->bfReserved1, fileHeader->bfReserved2, fileHeader->bfOffBits);
}

BMPINFOHEADER* criarInfoHeader(unsigned int biSize, int biWidth, int biHeight, unsigned short biPlanes, unsigned short biBitCount, unsigned int biCompression, unsigned int biSizeImage, int biXPelsPerMeter, int biYPelsPerMeter, unsigned int biClrUsed, unsigned int biClrImportant) {
    BMPINFOHEADER* infoHeader = (BMPINFOHEADER*) malloc(sizeof(BMPINFOHEADER));

    infoHeader->biSize = biSize;
    infoHeader->biWidth = biWidth;
    infoHeader->biHeight = biHeight;
    infoHeader->biPlanes = biPlanes;
    infoHeader->biBitCount = biBitCount;
    biCompression = biCompression;
    biSizeImage = biSizeImage;
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

void printInfoHeader(BMPINFOHEADER* infoHeader) {
    printf("biSize: %hu\nbiWidth: %d\nbiHeight: %d\nbiPlanes: %hu\nbiBitCount: %hu\nbiCompression: %hu\nbiSizeImage: %hu\nbiXPelsPerMeter: %d\nbiYPelsPerMeter: %d\nbiClrUsed: %hu\nbiClrImportant: %hu\n", infoHeader->biSize, infoHeader->biWidth, infoHeader->biHeight, infoHeader->biPlanes, infoHeader->biBitCount, infoHeader->biCompression, infoHeader->biSizeImage, infoHeader->biXPelsPerMeter, infoHeader->biYPelsPerMeter, infoHeader->biClrUsed, infoHeader->biClrImportant);
}

