#include "util.h"

int main() {
    FILE* img = fopen("./bmp_tresCores.bmp", "r");
    BMPFILEHEADER* fileHeader = leituraFileHeader(img);
    BMPINFOHEADER* infoHeader = leituraInfoHeader(img);
    
    printInfoHeader(infoHeader);

    for(int i = 0; i < 10000; i++) {
        PIXEL* pixel = leituraPixel(img);
        printPixel(pixel);
    }

    fclose(img);
}