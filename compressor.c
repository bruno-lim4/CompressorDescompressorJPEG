#include "./utils/header.h"
#include "./utils/imagem.h"


int main() {
    FILE* img_file = fopen("./imgs/bmp_tresCores.bmp", "r");
    IMAGEM* img = criarImagem(img_file);
    comprimeImagem(img);

    FILE* output_file = fopen("./imgs/compressed.bmp", "w");
    escreverImagem(output_file, img);
    fclose(output_file);
    fclose(img_file);
} 