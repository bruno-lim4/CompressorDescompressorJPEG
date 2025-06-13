#include "./utils/header.h"
#include "./utils/imagem.h"
#include "./utils/bloco.h"
#include "./utils/arvore.h"

int main(void){
    FILE* in = fopen("./bin/comprimido.bin", "rb");
    FILE* out = fopen("./imgs/descomprimido.bmp", "wb");

    IMAGEM* img = descomprimeImagem(in, out);
    salvarImagem(out, img);
    desalocarImagem(&img);

    fclose(in);
    fclose(out);

    return 0;
}