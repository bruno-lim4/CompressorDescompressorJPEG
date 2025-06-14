#include "arvore.h"
#include "leitor.h"
#include "decodifica.h"

ARVORE_AC* arvoreAC;
ARVORE_DC* arvoreDC;

void iniciarDecodificacao(){
    arvoreAC = criarArvoreAC();
    arvoreDC = criarArvoreDC();
}

void encerrarDecodificacao(){
    desalocarArvoreAC(&arvoreAC);
    desalocarArvoreDC(&arvoreDC);
}

void decodificaDC(int* DC, LEITOR* l, int DC_anterior, int* ehPrimeiroDC){ // Se ehPrimeiroDC == 1, estamos decodificando o primeiro.
    NO_DC* atual = getRaiz_DC(arvoreDC);

    while(!ehFolha_DC(atual)){
        int bit = lerBit(l);
        if(bit == 1)
            atual = getFilhoDireitoDC(atual);
        else
            atual = getFilhoEsquerdoDC(atual);
    }

    // Chegou na folha.
    int dif = lerValor(get_cat_DC(atual), l);

    if(*ehPrimeiroDC){
        *DC = dif;
        *ehPrimeiroDC = 0;
        return;
    }

    *DC = DC_anterior + dif;

    return;
}

void decodificaAC(int *bloco, LEITOR* l){
    int pos = 1; // Posição 0 já tem o coeficiente DC.
    while(pos < 64){ //Loop até preencher o bloco inteiro.
        NO_AC* atual = getRaiz_AC(arvoreAC);
        while(!ehFolha_AC(atual)){
            //printf("Debug loop\n");
            int bit = lerBit(l);
            if(bit == 1)
                atual = getFilhoDireitoAC(atual);
            else
                atual = getFilhoEsquerdoAC(atual);
        }

        // Chegou na folha.
        if(get_runlength_AC(atual) == 0 && get_size_AC(atual) == 0){ // EOB -> preencher com zeros até o final.
            for(int i = pos; i < 64; i++){
                bloco[i] = 0;
            }
            return;
        }
        else if(get_runlength_AC(atual) == 15 && get_size_AC(atual) == 0){ // Sequência de 15 zeros.
            for(int i = pos; i < pos + 15; i++){
                bloco[i] = 0;
            }
            pos += 15;
        }
        else{ //Preencher 'atual->runlength' posições com 0, e ler um valor de 'atual->size' bits.
            for(int i = pos; i < pos + get_runlength_AC(atual); i++){
                bloco[i] = 0;
            }

            pos += get_runlength_AC(atual);
            bloco[pos++] = lerValor(get_size_AC(atual), l);
        }
    }
}