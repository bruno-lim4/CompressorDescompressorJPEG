#include "./utils/header.h"
#include "./utils/imagem.h"
#include "./utils/bloco.h"
#include "./utils/arvore.h"

int main(void){

    ARVORE_AC* arv = criarArvoreAC();
    printAC(getRaiz_AC(arv));


    return 0;
}