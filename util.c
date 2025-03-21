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