#include "arvore.h"
#include "leitor.h"
#include "decodifica.h"

ARVORE_AC* arvoreAC;
ARVORE_DC* arvoreDC;
int criou_AC = 0;
int criou_DC = 0;

void decodificaDC(int* DC, LEITOR* l, int DC_anterior){ // Se DC_anterior == -1, estamos decodificando o primeiro.
    if(!criou_DC){
        arvoreDC = criarArvoreDC();
        criou_DC = 1;
    }

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
    if(DC_anterior == -1){
        *DC = dif;
        printf("DC: %d ", *DC);
        return;
    }

    *DC = DC_anterior + dif;

    printf("DC: %d ", *DC);

    return;
}

void decodificaAC(int *bloco, LEITOR* l){
    if(!criou_AC){
        arvoreAC = criarArvoreAC();
        criou_AC = 1;
    }

    printf("Sequência de bits lida:\n");

    int pos = 1;
    while(pos < 64){ //Loop até preencher o bloco inteiro.
        //printf("Debug 1\n");
        NO_AC* atual = getRaiz_AC(arvoreAC);
        //printf("Debug 2\n");
        while(!ehFolha_AC(atual)){
            //printf("Debug loop\n");
            int bit = lerBit(l);

            // Debug.
            if(bit)
                printf("1");
            else
                printf("0");

            if(bit == 1)
                atual = getFilhoDireitoAC(atual);
            else
                atual = getFilhoEsquerdoAC(atual);
            //ehFolha_AC(atual);
            //printf("Debug fim loop\n");
        }
        //printf("Debug 3\n");
        printf("\nrun: %d size: %d ehFolha: %d\n\n", get_runlength_AC(atual), get_size_AC(atual), ehFolha_AC(atual));

        // Debug.
        printf("AC: ");


        // Chegou na folha.
        if(get_runlength_AC(atual) == 0 && get_size_AC(atual) == 0){ // EOB -> preencher com zeros até o final.
            for(int i = pos; i < 64; i++){
                bloco[i] = 0;
                printf("%d ", bloco[i]);
            }
            printf("\n");
            pos = 64;
        }
        else if(get_runlength_AC(atual) == 15 && get_size_AC(atual) == 0){ // Sequência de 15 zeros.
            for(int i = pos; i < pos + 15; i++){
                bloco[i] = 0;
                printf("%d ", bloco[i]);
            }
            pos += 15;
        }
        else{ //Preencher 'atual->runlength' posições com 0, e ler um valor de 'atual->size' bits.
            for(int i = pos; i < pos + get_runlength_AC(atual); i++){
                bloco[i] = 0;
                printf("%d ", bloco[i]);
            }

            pos += get_runlength_AC(atual);
            bloco[pos++] = lerValor(get_size_AC(atual), l);
            printf("%d ", bloco[pos-1]);
        }
    }
    printf("\n");
}