#include "arvore.h"
#include "leitor.h"
#include "decodifica.h"
#include "util.h"

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

    int tamPref = 0, pref = 0;
    while(!ehFolha_DC(atual)){
        int bit = lerBit(l);
        
        tamPref++;
        pref <<= 1;
        pref |= bit;

        if(bit == 1)
            atual = getFilhoDireitoDC(atual);
        else
            atual = getFilhoEsquerdoDC(atual);
    }

    // Chegou na folha.
    int dif = lerValor(get_cat_DC(atual), l);

    printf("Decodifica DC:");
    printf(" | dif: %d | pref. lido: ", dif);
    print_binary(pref, tamPref);

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
        printf("Decodifica AC:");
        NO_AC* atual = getRaiz_AC(arvoreAC);
    
        int tamPref = 0, pref = 0;
        while(!ehFolha_AC(atual)){
            int bit = lerBit(l);

            tamPref++;
            pref <<= 1;
            pref |= bit;

            if(bit == 1)
                atual = getFilhoDireitoAC(atual);
            else
                atual = getFilhoEsquerdoAC(atual);
        }

        if(get_runlength_AC(atual) == 0 && get_size_AC(atual) == 0)
            printf(" | FIM DO BLOCO | ");
        else
            printf(" | (zeros = %d, size = %d) | ", get_runlength_AC(atual), get_size_AC(atual));

        if(pref == 0)
            printf("00\n");
        else if(pref == 1)
            printf("01\n");
        else
            print_binary(pref, tamPref);

        // Chegou na folha.
        if(get_runlength_AC(atual) == 0 && get_size_AC(atual) == 0){ // EOB -> preencher com zeros até o final.
            //printf("EOB! pos = %d\n", pos);
            for(int i = pos; i < 64; i++){
                bloco[i] = 0;
            }
            return;
        }
        else if(get_runlength_AC(atual) == 15 && get_size_AC(atual) == 0){ // Sequência de 15 zeros.
            //printf("15 zeros! pos = %d\n", pos);
            for(int i = pos; i < pos + 15; i++){
                bloco[i] = 0;
            }
            pos += 15;
        }
        else{ //Preencher 'atual->runlength' posições com 0, e ler um valor de 'atual->size' bits.
            //printf("(run, size) = (%d, %d)\n", get_runlength_AC(atual), get_size_AC(atual));
            //printf("runlength. pos = %d\n", pos);
            for(int i = pos; i < pos + get_runlength_AC(atual); i++){
                bloco[i] = 0;
            }

            pos += get_runlength_AC(atual);
            //printf("pos do valor = %d\n", pos);
            bloco[pos++] = lerValor(get_size_AC(atual), l);
            printf("valor = %d\n", bloco[pos-1]);
        }
    }
}