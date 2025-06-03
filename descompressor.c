#include "./utils/header.h"
#include "./utils/imagem.h"
#include "./utils/bloco.h"

int main(void){
    FILE* img_file = fopen("./imgs/bmp_tresCores.bmp", "r");
    IMAGEM* img = criarImagem(img_file);

    //printf("%d\n", get_img_w(img));

    double ***b = malloc(25*sizeof(double**));
    for(int i = 0; i < 25; i++)
        alocarMatriz_double(&(b[i]), 8, 8);

    for(int i = 0; i < 25; i++){
        for(int j = 0; j < 8; j++){
            for(int k = 0; k < 8; k++){
                (b[i])[j][k] = i;
            }
        }
    }

    IMAGEM *img_res = construirImagem(b, 16, 400);
    
    printarImagem(img_res);

    return 0;
}