#include "arvore.h"
#include "consts.h"

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
        if(tabelaAC[i][j] & (1 << k))
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
    ARVORE_AC* arv = malloc(sizeof(ARVORE_AC));
    arv->raiz = criarNo_AC(-1, -1);

    for(int runlength = 0; runlength < 16; runlength++){
        for(int size = 0; size < 10; size++){
            inserirPrefixoAC(arv, tabelaAC[runlength][size], calculaComprimento_AC(runlength, size), runlength, size+1);
        }
    }

    // EOB.
    inserirPrefixoAC(arv, tabelaAC_acabouVetor, 4, 0, 0);
    // Sequência de 15 zeros.
    inserirPrefixoAC(arv, tabelaAC_quinzeZeros, 12, 15, 0);

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