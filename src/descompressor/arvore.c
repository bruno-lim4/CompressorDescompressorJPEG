#include "arvore.h"

uint32_t tabela_AC[16][10];
int iniciou_tabelaAC = 0;

void iniciarTabelaAC(){
    tabela_AC[0][0] = 0b00;
    tabela_AC[0][1] = 0b01;
    tabela_AC[0][2] = 0b100;
    tabela_AC[0][3] = 0b1011;
    tabela_AC[0][4] = 0b11010;
    tabela_AC[0][5] = 0b111000;
    tabela_AC[0][6] = 0b1111000;
    tabela_AC[0][7] = 0b1111110110;
    tabela_AC[0][8] = 0b1111111110000010;
    tabela_AC[0][9] = 0b1111111110000011;

    tabela_AC[1][0] = 0b1100;
    tabela_AC[1][1] = 0b111001;
    tabela_AC[1][2] = 0b1111001;
    tabela_AC[1][3] = 0b111110110;
    tabela_AC[1][4] = 0b11111110110;
    tabela_AC[1][5] = 0b1111111110000100;
    tabela_AC[1][6] = 0b1111111110000101;
    tabela_AC[1][7] = 0b1111111110000110;
    tabela_AC[1][8] = 0b1111111110000111;
    tabela_AC[1][9] = 0b1111111110001000;

    tabela_AC[2][0] = 0b11011;
    tabela_AC[2][1] = 0b11111000;
    tabela_AC[2][2] = 0b1111110111;
    tabela_AC[2][3] = 0b1111111110001001;
    tabela_AC[2][4] = 0b1111111110001010;
    tabela_AC[2][5] = 0b1111111110001011;
    tabela_AC[2][6] = 0b1111111110001100;
    tabela_AC[2][7] = 0b1111111110001101;
    tabela_AC[2][8] = 0b1111111110001110;
    tabela_AC[2][9] = 0b1111111110001111;

    tabela_AC[3][0] = 0b111010;
    tabela_AC[3][1] = 0b111110111;
    tabela_AC[3][2] = 0b11111110111;
    tabela_AC[3][3] = 0b1111111110010000;
    tabela_AC[3][4] = 0b1111111110010001;
    tabela_AC[3][5] = 0b1111111110010010;
    tabela_AC[3][6] = 0b1111111110010011;
    tabela_AC[3][7] = 0b1111111110010100;
    tabela_AC[3][8] = 0b1111111110010101;
    tabela_AC[3][9] = 0b1111111110010110;

    tabela_AC[4][0] = 0b111011;
    tabela_AC[4][1] = 0b1111111000;
    tabela_AC[4][2] = 0b1111111110010111;
    tabela_AC[4][3] = 0b1111111110011000;
    tabela_AC[4][4] = 0b1111111110011001;
    tabela_AC[4][5] = 0b1111111110011010;
    tabela_AC[4][6] = 0b1111111110011011;
    tabela_AC[4][7] = 0b1111111110011100;
    tabela_AC[4][8] = 0b1111111110011101;
    tabela_AC[4][9] = 0b1111111110011110;

    tabela_AC[5][0] = 0b1111010;
    tabela_AC[5][1] = 0b1111111001;
    tabela_AC[5][2] = 0b1111111110011111;
    tabela_AC[5][3] = 0b1111111110100000;
    tabela_AC[5][4] = 0b1111111110100001;
    tabela_AC[5][5] = 0b1111111110100010;
    tabela_AC[5][6] = 0b1111111110100011;
    tabela_AC[5][7] = 0b1111111110100100;
    tabela_AC[5][8] = 0b1111111110100101;
    tabela_AC[5][9] = 0b1111111110100110;

    tabela_AC[6][0] = 0b1111011;
    tabela_AC[6][1] = 0b11111111000;
    tabela_AC[6][2] = 0b1111111110100111;
    tabela_AC[6][3] = 0b1111111110101000;
    tabela_AC[6][4] = 0b1111111110101001;
    tabela_AC[6][5] = 0b1111111110101010;
    tabela_AC[6][6] = 0b1111111110101011;
    tabela_AC[6][7] = 0b1111111110101100;
    tabela_AC[6][8] = 0b1111111110101101;
    tabela_AC[6][9] = 0b1111111110101110;

    tabela_AC[7][0] = 0b11111001;
    tabela_AC[7][1] = 0b11111111001;
    tabela_AC[7][2] = 0b1111111110101111;
    tabela_AC[7][3] = 0b1111111110110000;
    tabela_AC[7][4] = 0b1111111110110001;
    tabela_AC[7][5] = 0b1111111110110010;
    tabela_AC[7][6] = 0b1111111110110011;
    tabela_AC[7][7] = 0b1111111110110100;
    tabela_AC[7][8] = 0b1111111110110101;
    tabela_AC[7][9] = 0b1111111110110110;

    tabela_AC[8][0] = 0b11111010;
    tabela_AC[8][1] = 0b111111111000000;
    tabela_AC[8][2] = 0b1111111110110111;
    tabela_AC[8][3] = 0b1111111110111000;
    tabela_AC[8][4] = 0b1111111110111001;
    tabela_AC[8][5] = 0b1111111110111010;
    tabela_AC[8][6] = 0b1111111110111011;
    tabela_AC[8][7] = 0b1111111110111100;
    tabela_AC[8][8] = 0b1111111110111101;
    tabela_AC[8][9] = 0b1111111110111110;

    tabela_AC[9][0] = 0b111111000;
    tabela_AC[9][1] = 0b1111111110111111;
    tabela_AC[9][2] = 0b1111111111000000;
    tabela_AC[9][3] = 0b1111111111000001;
    tabela_AC[9][4] = 0b1111111111000010;
    tabela_AC[9][5] = 0b1111111111000011;
    tabela_AC[9][6] = 0b1111111111000100;
    tabela_AC[9][7] = 0b1111111111000101;
    tabela_AC[9][8] = 0b1111111111000110;
    tabela_AC[9][9] = 0b1111111111000111;

    tabela_AC[10][0] = 0b111111001;
    tabela_AC[10][1] = 0b1111111111001000;
    tabela_AC[10][2] = 0b1111111111001001;
    tabela_AC[10][3] = 0b1111111111001010;
    tabela_AC[10][4] = 0b1111111111001011;
    tabela_AC[10][5] = 0b1111111111001100;
    tabela_AC[10][6] = 0b1111111111001101;
    tabela_AC[10][7] = 0b1111111111001110;
    tabela_AC[10][8] = 0b1111111111001111;
    tabela_AC[10][9] = 0b1111111111010000;

    tabela_AC[11][0] = 0b111111010;
    tabela_AC[11][1] = 0b1111111111010001;
    tabela_AC[11][2] = 0b1111111111010010;
    tabela_AC[11][3] = 0b1111111111010011;
    tabela_AC[11][4] = 0b1111111111010100;
    tabela_AC[11][5] = 0b1111111111010101;
    tabela_AC[11][6] = 0b1111111111010110;
    tabela_AC[11][7] = 0b1111111111010111;
    tabela_AC[11][8] = 0b1111111111011000;
    tabela_AC[11][9] = 0b1111111111011001;

    tabela_AC[12][0] = 0b1111111010;
    tabela_AC[12][1] = 0b1111111111011010;
    tabela_AC[12][2] = 0b1111111111011011;
    tabela_AC[12][3] = 0b1111111111011100;
    tabela_AC[12][4] = 0b1111111111011101;
    tabela_AC[12][5] = 0b1111111111011110;
    tabela_AC[12][6] = 0b1111111111011111;
    tabela_AC[12][7] = 0b1111111111100000;
    tabela_AC[12][8] = 0b1111111111100001;
    tabela_AC[12][9] = 0b1111111111100010;

    tabela_AC[13][0] = 0b11111111010;
    tabela_AC[13][1] = 0b1111111111100011;
    tabela_AC[13][2] = 0b1111111111100100;
    tabela_AC[13][3] = 0b1111111111100101;
    tabela_AC[13][4] = 0b1111111111100110;
    tabela_AC[13][5] = 0b1111111111100111;
    tabela_AC[13][6] = 0b1111111111101000;
    tabela_AC[13][7] = 0b1111111111101001;
    tabela_AC[13][8] = 0b1111111111101010;
    tabela_AC[13][9] = 0b1111111111101011;

    tabela_AC[14][0] = 0b111111110110;
    tabela_AC[14][1] = 0b1111111111101100;
    tabela_AC[14][2] = 0b1111111111101101;
    tabela_AC[14][3] = 0b1111111111101110;
    tabela_AC[14][4] = 0b1111111111101111;
    tabela_AC[14][5] = 0b1111111111110000;
    tabela_AC[14][6] = 0b1111111111110001;
    tabela_AC[14][7] = 0b1111111111110010;
    tabela_AC[14][8] = 0b1111111111110011;
    tabela_AC[14][9] = 0b1111111111110100;

    tabela_AC[15][0] = 0b1111111111110101;
    tabela_AC[15][1] = 0b1111111111110110;
    tabela_AC[15][2] = 0b1111111111110111;
    tabela_AC[15][3] = 0b1111111111111000;
    tabela_AC[15][4] = 0b1111111111111001;
    tabela_AC[15][5] = 0b1111111111111010;
    tabela_AC[15][6] = 0b1111111111111011;
    tabela_AC[15][7] = 0b1111111111111100;
    tabela_AC[15][8] = 0b1111111111111101;
    tabela_AC[15][9] = 0b1111111111111110;

    iniciou_tabelaAC = 1;
}

uint32_t tabela_DC[11];
int iniciou_tabelaDC;

void iniciarTabelaDC(){
    tabela_DC[0] = 0b010;
    tabela_DC[1] = 0b011;
    tabela_DC[2] = 0b100;
    tabela_DC[3] = 0b00;
    tabela_DC[4] = 0b101;
    tabela_DC[5] = 0b110;
    tabela_DC[6] = 0b1110;
    tabela_DC[7] = 0b11110;
    tabela_DC[8] = 0b111110;
    tabela_DC[9] = 0b1111110;
    tabela_DC[10] = 0b11111110;

    iniciou_tabelaDC = 1;
}

struct no_dc_{
    struct no_dc_* esquerda;
    struct no_dc_* direita;
    int ehFolha;
    int cat; // categoria.
};

struct arvore_dc_{
    struct no_dc_* raiz;
};

struct no_ac_ {
    struct no_ac_* esquerda;
    struct no_ac_* direita;
    int ehFolha;
    int runlength;
    int size;
};

struct arvore_ac_{
    struct no_ac_* raiz;
};

int calculaComprimento_DC(int i){
    if(i == 3)
        return 2;
    if(i == 0 || i == 1)
        return 3;

    int ultimo_um = 0;
    for(int k = 0; k < 32; k++){
        if(tabela_DC[i] & (1 << k))
            ultimo_um = k; // guarda posição do último bit igual a 1.
    }

    return ultimo_um+1;    
}

int calculaComprimento_AC(int i, int j){
    if(i == 0 && (j == 0 || j == 1))
        return 2;

    int ultimo_um = 0;
    for(int k = 0; k < 32; k++){
        if(tabela_AC[i][j] & (1 << k))
            ultimo_um = k; // guarda posição do último bit igual a 1.
    }

    return ultimo_um+1;
}

ARVORE_DC* criarArvoreDC(){
    if(!iniciou_tabelaDC)
        iniciarTabelaDC();

    ARVORE_DC* arv = malloc(sizeof(ARVORE_DC));
    arv->raiz = criarNo_DC(-1);

    for(int cat = 0; cat < 11; cat++){
        inserirCatDC(arv, tabela_DC[cat], calculaComprimento_DC(cat), cat);
    }

    return arv;
}

NO_DC* criarNo_DC(int cat){
    NO_DC* no = malloc(sizeof(NO_DC));

    no->esquerda = no->direita = NULL;
    no->ehFolha = 0;
    no->cat = cat;

    return no;
}

void inserirCatDC(ARVORE_DC* arv, int prefixo, int comprimento, int cat){
    NO_DC* atual = arv->raiz;
    for (int i = comprimento - 1; i >= 0; i--) {
        int bit = (prefixo >> i) & 1; // vai pegando os bits do prefixo, da esquerda pra direita, pra percorrer a árvore.
        if (bit == 0) {
            if (!atual->esquerda) {
                atual->esquerda = criarNo_DC(-1);
            }
            atual = atual->esquerda;
        } else {
            if (!atual->direita) {
                atual->direita = criarNo_DC(-1);
            }
            atual = atual->direita;
        }
    }
    // Chegou na folha.
    atual->cat = cat;
    atual->ehFolha = 1;
}

NO_DC* getRaiz_DC(ARVORE_DC* arv){
    return arv->raiz;
}

NO_DC* getFilhoEsquerdoDC(NO_DC* no){
    return no->esquerda;
}

NO_DC* getFilhoDireitoDC(NO_DC* no){
    return no->direita;
}

int get_cat_DC(NO_DC* no){
    return no->cat;
}

int ehFolha_DC(NO_DC* no){
    return no->ehFolha;
}

void desalocarNosDC(NO_DC* no){
    if (no == NULL)
        return;
    
    desalocarNosDC(no->esquerda);    
    desalocarNosDC(no->direita);
    
    free(no);
}

void desalocarArvoreDC(ARVORE_DC** arv){
    desalocarNosDC((*arv)->raiz);
    free(*arv);
    *arv = NULL;
}


ARVORE_AC* criarArvoreAC(){
    if(!iniciou_tabelaAC)
        iniciarTabelaAC();

    ARVORE_AC* arv = malloc(sizeof(ARVORE_AC));
    arv->raiz = criarNo_AC(-1, -1);

    for(int runlength = 0; runlength < 16; runlength++){
        for(int size = 0; size < 10; size++){
            inserirPrefixoAC(arv, tabela_AC[runlength][size], calculaComprimento_AC(runlength, size), runlength, size+1);
        }
    }

    // EOB.
    inserirPrefixoAC(arv, 0b1010, 4, 0, 0);
    // Sequência de 15 zeros.
    inserirPrefixoAC(arv, 0b11111111011, 11, 15, 0);

    return arv;
}


NO_AC* criarNo_AC(int runlength, int size){
    NO_AC* no = malloc(sizeof(NO_AC));

    no->esquerda = no->direita = NULL;
    no->ehFolha = 0;
    no->runlength = runlength;
    no->size = size;

    return no;
}

void inserirPrefixoAC(ARVORE_AC* arv, int prefixo, int comprimento, int run_length, int size){
    NO_AC* atual = arv->raiz;
    for (int i = comprimento - 1; i >= 0; i--) {
        int bit = (prefixo >> i) & 1; // vai pegando os bits do prefixo, da esquerda pra direita, pra percorrer a árvore.
        if (bit == 0) {
            if (!atual->esquerda) {
                atual->esquerda = criarNo_AC(-1, -1);
            }
            atual = atual->esquerda;
        } else {
            if (!atual->direita) {
                atual->direita = criarNo_AC(-1, -1);
            }
            atual = atual->direita;
        }
    }
    // Chegou na folha.
    atual->runlength = run_length;
    atual->size = size;
    atual->ehFolha = 1;
}

NO_AC* getRaiz_AC(ARVORE_AC* arv){
    return arv->raiz;
}

NO_AC* getFilhoEsquerdoAC(NO_AC* no){
    return no->esquerda;
}

NO_AC* getFilhoDireitoAC(NO_AC* no){
    return no->direita;
}

int get_runlength_AC(NO_AC* no){
    return no->runlength;
}

int get_size_AC(NO_AC* no){
    return no->size;
}

int ehFolha_AC(NO_AC* no){
    return no->ehFolha;
}

void desalocarNosAC(NO_AC* no){
    if (no == NULL)
        return;
    
    desalocarNosAC(no->esquerda);    
    desalocarNosAC(no->direita);
    
    free(no);
}

void desalocarArvoreAC(ARVORE_AC** arv){
    desalocarNosAC((*arv)->raiz);
    free(*arv);
    *arv = NULL;
}

void printAC_rec(NO_AC* raiz){
    if(raiz == NULL)
        return;

    printf("runlength: %d size: %d ehFolha: %d\n", raiz->runlength, raiz->size, raiz->ehFolha);

    printAC_rec(raiz->esquerda);
    printAC_rec(raiz->direita);
}

void printAC(ARVORE_AC* arv){
    printAC_rec(arv->raiz);
}

void printDC_rec(NO_DC* raiz){
    if(raiz == NULL)
        return;

    //if(ehFolha_DC(raiz)){
        printf("cat: %d ehFolha: %d\n", raiz->cat, raiz->ehFolha);
    //}

    printDC_rec(raiz->esquerda);
    printDC_rec(raiz->direita);
}
void printDC(ARVORE_DC* arv){
    printDC_rec(arv->raiz);
}

int calcularCompInt(int pref){
    if(pref <= 1)
        return 2;
    int ultimo_um = 0;
    for(int k = 0; k < 32; k++){
        if(pref & (1 << k))
            ultimo_um = k; // guarda posição do último bit igual a 1.
    }

    return ultimo_um+1;    
}

NO_AC* buscarPrefixo(int pref, NO_AC* atual){
    /*
    int comp, pref;
    if(i == 0 && j == 0){
        comp = 4;
        pref = 0b1010;
    }
    else if(i == 15 && j == 0){
        comp = 11;
        pref = 0b11111111011;
    }
    else{
        comp = calculaComprimento_AC(i, j);
        pref = tabela_AC[i][j-1];
    }
    */
    int comp = calcularCompInt(pref);
    printf("comprimento: %d\n", comp);
    for(int k = 0; k < comp; k++){
        if(atual == NULL){
            //printf("NULO! i: %d\n", k);
            return NULL;
        }
        if(pref & (1 << (comp - 1 - k))){
            if(pref == 0){
                printf("DESCEU ESQ\n");
            }
            //printf("shift: %d tabela: %d direita\n", 1 << (comp - 1 - k), pref);
            atual = getFilhoDireitoAC(atual);
        }
        else{
            if(pref == 0){
                printf("DESCEU DIR\n");
            }

            //printf("shift: %d tabela: %d esquerda\n", 1 << (comp - 1 - k), pref);
            atual = getFilhoEsquerdoAC(atual);
        }
    }
    //printf("na buscar: run: %d, size: %d\n", get_runlength_AC(atual), get_size_AC(atual));

    return atual;
}