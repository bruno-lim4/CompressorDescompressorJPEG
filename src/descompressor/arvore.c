#include "arvore.h"
#include "consts.h"

// Structs que definem as árvores.
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

int calculaComprimento_DC(int i){ // Calcula comprimento do prefixo na posição i da tabela DC.
    if(i == 3)
        return 2;
    if(i == 0 || i == 1)
        return 3;

    int ultimo_um = 0;
    for(int k = 0; k < 32; k++){
        if(tabelaDC[i] & (1 << k))
            ultimo_um = k; // guarda posição do último bit igual a 1.
    }

    return ultimo_um+1;    
}

int calculaComprimento_AC(int i, int j){ // Calcula comprimento do prefixo na posição [i][j] da tabela AC.
    if(i == 0 && (j == 0 || j == 1))
        return 2;

    int ultimo_um = 0;
    for(int k = 0; k < 32; k++){
        if(tabelaAC[i][j] & (1 << k))
            ultimo_um = k; // guarda posição do último bit igual a 1.
    }

    return ultimo_um+1;
}

ARVORE_DC* criarArvoreDC(){ // Inicializa a árvore.

    ARVORE_DC* arv = malloc(sizeof(ARVORE_DC));
    arv->raiz = criarNo_DC(-1);

    for(int cat = 0; cat < 11; cat++){
        inserirCatDC(arv, tabelaDC[cat], calculaComprimento_DC(cat), cat);
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

void inserirCatDC(ARVORE_DC* arv, int prefixo, int comprimento, int cat){ // Insere um nó na árvore DC.
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


ARVORE_AC* criarArvoreAC(){ // Inicializa a árvore.
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

void inserirPrefixoAC(ARVORE_AC* arv, int prefixo, int comprimento, int run_length, int size){ // Insere nó na árvore AC.
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