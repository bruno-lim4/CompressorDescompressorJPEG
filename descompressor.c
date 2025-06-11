#include "./utils/header.h"
#include "./utils/imagem.h"
#include "./utils/bloco.h"
#include "./utils/arvore.h"

int main(void){
    FILE* in = fopen("./bin/carneiro.bin", "rb");
    FILE* out = fopen("./imgs/carneiro_descomp.bmp", "wb");

/*
    ARVORE_AC* arv = criarArvoreAC();
    printAC(arv);
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

    //IMAGEM* original = criarImagem(in);
    //fseek(in, 0, SEEK_SET);
    IMAGEM* img = descomprimeImagem(in, out);

    /*
    ARVORE_AC* arv = criarArvoreAC();
    NO_AC* atual = getRaiz_AC(arv);
    atual = buscarPrefixo(0b11111011, atual);
    printf("run: %d, size: %d\n", get_runlength_AC(atual), get_size_AC(atual));

    if(getFilhoDireitoAC(atual) != NULL){
        printf("Direito: run: %d, size: %d\n", get_runlength_AC(getFilhoDireitoAC(atual)), get_size_AC(getFilhoDireitoAC(atual)));
    }
    if(getFilhoEsquerdoAC(atual) != NULL){
        printf("Esquerdo: run: %d, size: %d\n", get_runlength_AC(getFilhoEsquerdoAC(atual)), get_size_AC(getFilhoEsquerdoAC(atual)));
    }
    */
    //printf("original:\n");
    //printarImagem(original);
    //printf("\n\n========================================\nimg:\n");
    //printarImagem(img);

    salvarBMP(out, img);

    fclose(in);
    fclose(out);

    return 0;
}