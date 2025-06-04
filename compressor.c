#include "./utils/header.h"
#include "./utils/imagem.h"
#include "./utils/codifica.h"

int main() {
    FILE* img_file = fopen("./imgs/carneiro.bmp", "rb");
    IMAGEM* img = criarImagem(img_file);

    FILE* bin_file = fopen("./bin/carneiro.bin", "wb");
    comprimeImagem(img, bin_file);

    fclose(img_file);
    fclose(bin_file);
} 