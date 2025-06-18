#include "arvore.h"
#include "leitor.h"
#include "decodifica.h"

ARVORE_AC* arvoreAC;
ARVORE_DC* arvoreDC;

void iniciarDecodificacao(){ // Cria as árvores de prefixos AC e DC.
    arvoreAC = criarArvoreAC();
    arvoreDC = criarArvoreDC();
}

void encerrarDecodificacao(){ // Desaloca as árvores.
    desalocarArvoreAC(&arvoreAC);
    desalocarArvoreDC(&arvoreDC);
}

// Lê o valor codificado no arquivo de entrada e retorna o valor do coeficiente DC.
void decodificaDC(int* DC, LEITOR* l, int DC_anterior, int* ehPrimeiroDC){ // Se ehPrimeiroDC == 1, estamos decodificando o primeiro.
    NO_DC* atual = getRaiz_DC(arvoreDC);

    while(!ehFolha_DC(atual)){ // Percorre a árvore até chegar numa folha.
        int bit = lerBit(l); // Lê um bit do arquivo.
        if(bit == 1) // Se bit == 1, desce para a direita.
            atual = getFilhoDireitoDC(atual);
        else // Se bit == 0, desce para a esquerda.
            atual = getFilhoEsquerdoDC(atual);
    }

    // Chegou na folha.
    int dif = lerValor(get_cat_DC(atual), l); // Lê o valor da diferença codificado no arquivo.

    if(*ehPrimeiroDC){ // Se for o primeiro DC, então o coeficiente é igual à diferença decodificada.
        *DC = dif;
        *ehPrimeiroDC = 0;
        return;
    }

    *DC = DC_anterior + dif;

    return;
}

void decodificaAC(int *bloco, LEITOR* l){ // Decodifica os 63 valores AC do bloco.
    int pos = 1; // Posição 0 já tem o coeficiente DC.
    while(pos < 64){ //Loop até preencher o bloco inteiro.
        NO_AC* atual = getRaiz_AC(arvoreAC);
        while(!ehFolha_AC(atual)){ // Percorrendo a árvore.
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