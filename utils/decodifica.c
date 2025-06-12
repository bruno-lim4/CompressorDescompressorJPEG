#include "arvore.h"
#include "leitor.h"
#include "decodifica.h"
#include "imagem.h"

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
    int debug = 0;
    while(!ehFolha_DC(atual)){
        int bit = lerBit(l);
        debug <<= 1;
        debug |= bit;
        if(bit == 1)
            atual = getFilhoDireitoDC(atual);
        else
            atual = getFilhoEsquerdoDC(atual);
    }

    //printf("\nsequência lida: ");
    //printarInt(debug);
    // Chegou na folha.
    int dif = lerValor(get_cat_DC(atual), l);
    //printf("valor lido: ");
    //printarInt(dif);
    //printf("\n");

    if(*ehPrimeiroDC){
        *DC = dif;
        *ehPrimeiroDC = 0;
        //printf("DC: %d ", *DC);
        return;
    }

    *DC = DC_anterior + dif;

    //printf("DC: %d\n", *DC);

    return;
}

void decodificaAC(int *bloco, LEITOR* l){
    //printf("Sequência de bits lida:\n");

    int pos = 1;
    while(pos < 64){ //Loop até preencher o bloco inteiro.
        //printf("pos: %d\n", pos);
        //printf("Debug 1\n");
        NO_AC* atual = getRaiz_AC(arvoreAC);
        //printf("Debug 2\n");
        int debug = 0;
        while(!ehFolha_AC(atual)){
            //printf("Debug loop\n");
            int bit = lerBit(l);
            debug <<= 1;
            debug |= bit;

            // Debug.
            /*
            if(bit)
                printf("1\n");
            else
                printf("0\n");
            */

            if(bit == 1)
                atual = getFilhoDireitoAC(atual);
            else
                atual = getFilhoEsquerdoAC(atual);
            //ehFolha_AC(atual);
            //printf("Debug fim loop\n");
        }
        //printf("Debug 3\n");
        //printf("\nrun: %d size: %d ehFolha: %d\n\n", get_runlength_AC(atual), get_size_AC(atual), ehFolha_AC(atual));

        // Debug.
        //printf("AC: ");


        // Chegou na folha.

        //printf("sequência lida: ");
        //printarInt(debug);

        if(get_runlength_AC(atual) == 0 && get_size_AC(atual) == 0){ // EOB -> preencher com zeros até o final.
            //printf("EOB!\n");
            for(int i = pos; i < 64; i++){
                bloco[i] = 0;
                //printf("%d ", bloco[i]);
            }
            //printf("\n");
            return;
        }
        else if(get_runlength_AC(atual) == 15 && get_size_AC(atual) == 0){ // Sequência de 15 zeros.
            //printf("15 zeros!\n");
            for(int i = pos; i < pos + 15; i++){
                bloco[i] = 0;
                //printf("%d ", bloco[i]);
            }
            //printf("\n");
            pos += 15;
        }
        else{ //Preencher 'atual->runlength' posições com 0, e ler um valor de 'atual->size' bits.
            for(int i = pos; i < pos + get_runlength_AC(atual); i++){
                bloco[i] = 0;
                //printf("%d ", bloco[i]);
            }

            //printf("\n");
            pos += get_runlength_AC(atual);
            // debug
            int valor_lido = bloco[pos++] = lerValor(get_size_AC(atual), l);
            //printf("valor lido: ");
            //printarInt(valor_lido);
            //printf("\n");

            //printf("%d ", bloco[pos-1]);
            //printf("%d zeros e valor %d, pos é %d\n", get_runlength_AC(atual), debug, pos);
        }
    }
    //printf("\n");
}