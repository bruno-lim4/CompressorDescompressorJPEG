#include "util.h"

int main() {
    FILE* img = fopen("./bmp_tresCores.bmp", "r");
    BMPFILEHEADER* fileHeader = leituraFileHeader(img);
    BMPINFOHEADER* infoHeader = leituraInfoHeader(img);
    
    printFileHeader(fileHeader);
    printInfoHeader(infoHeader);

    for(int i = 0; i < 10; i++) {
        PIXEL* pixel = leituraPixel(img);
        printPixel(pixel);
    }

    fclose(img);
}