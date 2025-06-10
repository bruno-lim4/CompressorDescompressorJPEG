#include "./utils/header.h"
#include "./utils/imagem.h"
#include "./utils/bloco.h"
#include "./utils/arvore.h"

int main(void){
    FILE* in = fopen("./bin/carneiro.bin", "rb");
    FILE* out = fopen("./imgs/carneiro_descomp.bmp", "wb");

/*
    ARVORE_DC* arv = criarArvoreDC();
    printDC(arv);
*/
/*
    // Debug.
    fseek(in, 54, SEEK_SET);
    uint32_t buffer;
    fread(&buffer, sizeof(uint32_t), 1, in);
    printarUint_32(buffer);
    fread(&buffer, sizeof(uint32_t), 1, in);
    printarUint_32(buffer);
    fread(&buffer, sizeof(uint32_t), 1, in);
    printarUint_32(buffer);
    fread(&buffer, sizeof(uint32_t), 1, in);
    printarUint_32(buffer);
    fread(&buffer, sizeof(uint32_t), 1, in);
    printarUint_32(buffer);
    fread(&buffer, sizeof(uint32_t), 1, in);
    printarUint_32(buffer);   
*/

    IMAGEM* original = criarImagem(in);
    fseek(in, 0, SEEK_SET);
    IMAGEM* img = descomprimeImagem(in, out);

    //printf("original:\n");
    //printarImagem(original);
    //printf("\n\n========================================\nimg:\n");
    //printarImagem(img);

    salvarBMP(out, img);


    fclose(in);
    fclose(out);

    return 0;
}