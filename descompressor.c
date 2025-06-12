#include "./utils/header.h"
#include "./utils/imagem.h"
#include "./utils/bloco.h"
#include "./utils/arvore.h"

int main(void){
    FILE* in = fopen("./bin/carneiro.bin", "rb");
    FILE* out = fopen("./imgs/carneiro_descomp.bmp", "wb");

    //printf("Cabeçalhos da in:\n");
    //IMAGEM* img_in = criarImagem(in);
    //printf("\n\nCabeçalhos da out:\n");
    //IMAGEM* img_out = criarImagem(out);

    //fseek(in, 0, SEEK_SET);
    //fseek(out, 0, SEEK_SET);

    IMAGEM* img = descomprimeImagem(in, out);

    salvarBMP(out, img);

    desalocarImagem(&img);
    
    //fseek(out, 0, SEEK_SET);
    //IMAGEM* img_out = criarImagem(out);

    fclose(in);
    fclose(out);

    return 0;
}