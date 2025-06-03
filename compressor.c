#include "./utils/header.h"
#include "./utils/imagem.h"
#include "./utils/gravar.h"

int main() {
    // debug
    int qtd;
    uint32_t res = grava_infoAC(15, 0, &qtd);
    print_binary(res, qtd);

    FILE* img_file = fopen("./imgs/carneiro.bmp", "r");
    IMAGEM* img = criarImagem(img_file);
    comprimeImagem(img);

    FILE* output_file = fopen("./imgs/compressed.bmp", "w");
    escreverImagem(output_file, img);
    fclose(output_file);
    fclose(img_file);
} 