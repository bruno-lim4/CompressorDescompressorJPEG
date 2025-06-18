#include "codifica.h"
#include "util.h"
#include "consts.h"

uint32_t codifica_infoDC(int value, int* qtd_bits) {
    int value_abs = abs(value);
    char cat = 'X';

    // pega a categoria
    if (value_abs == 0) cat = '0';
    else if (value_abs == 1) cat = '1';
    else if (value_abs <= 3) cat = '2';
    else if (value_abs <= 7) cat = '3';
    else if (value_abs <= 15) cat = '4';
    else if (value_abs <= 31) cat = '5';
    else if (value_abs <= 63) cat = '6';
    else if (value_abs <= 127) cat = '7';
    else if (value_abs <= 255) cat = '8';
    else if (value_abs <= 511) cat = '9';
    else if (value_abs <= 1023) cat = 'A';
    else cat = 'A';

    uint32_t res = 0;
    int qtd = 0;

    // guarda o prefixo correspondente
    switch (cat)
    {
    case '0':
        res = shifta_e_grava(res, tabelaDC[0], 3);
        qtd = 3;
        break;
    
    case '1':
        res = shifta_e_grava(res, tabelaDC[1], 3);
        qtd = 3;
        break;
    
    case '2':
        res = shifta_e_grava(res, tabelaDC[2], 3);
        qtd = 3;
        break;
    
    case '3':
        res = shifta_e_grava(res, tabelaDC[3], 2);
        qtd = 2;
        break;
    
    case '4':
        res = shifta_e_grava(res, tabelaDC[4], 3);
        qtd = 3;
        break;

    case '5':
        res = shifta_e_grava(res, tabelaDC[5], 3);
        qtd = 3;
        break;
    
    case '6':
        res = shifta_e_grava(res, tabelaDC[6], 4);
        qtd = 4;
        break;
    
    case '7':
        res = shifta_e_grava(res, tabelaDC[7], 5);
        qtd = 5;
        break;
    
    case '8':
        res = shifta_e_grava(res, tabelaDC[8], 6);
        qtd = 6;
        break;
    
    case '9':
        res = shifta_e_grava(res, tabelaDC[9], 7);
        qtd = 7;
        break;
    
    case 'A':
        res = shifta_e_grava(res, tabelaDC[10], 8);
        qtd = 8;
        break;
    
    default:
        break;
    }

    // guarda a mantissa em complemento de 1
    int qtd_m;
    uint32_t mantissa_complemento1 = get_mantissa_comp1(value, &qtd_m);

    // coloca como sufixo
    res = shifta_e_grava(res, mantissa_complemento1, qtd_m);
    qtd += qtd_m;

    *qtd_bits = qtd;
    return res;
}

// dado o numero de zeros e a categoria, acha o prefixo e retorna, e tambem muda 
// a qtd_bits para ser o valor de bits validos nesse prefixo
uint32_t get_prefixoAC(int zeros, char cat, int* qtd_bits) {
    // se a categoria for 0, a gente considera esses casos especiais
    if (cat == '0') {
        if (zeros == 15) {
            *qtd_bits = 12;
            return tabelaAC_quinzeZeros;
        } else if (zeros == 0) {
            *qtd_bits = 4;
            return tabelaAC_acabouVetor;
        }
    } 

    // faz algumas transformacao de index pra ser valido da forma q a gente guardou a tabela

    int j;

    if (cat == 'A') {
        j = 9;
    } else {
        j = (int) ((int)cat-(int)'0');
    }

    uint32_t pref = tabelaAC[zeros][j-1];

    *qtd_bits = get_qtdBits(pref);

    // nesse caso (cat = 1 ou 2), o prefixo comeca com 0
    if (zeros == 0 && (cat=='1' || cat=='2')) {
        *qtd_bits += 1;
        if (cat=='1') *qtd_bits += 1;
    }

    return pref;
}


uint32_t codifica_infoAC(int qtd_zero, int value, int* qtd_bits) {
    // acho a categoria correspondente
    int value_abs = abs(value);
    char cat = 'X';

    if (value_abs == 0) cat = '0';
    else if (value_abs == 1) cat = '1';
    else if (value_abs <= 3) cat = '2';
    else if (value_abs <= 7) cat = '3';
    else if (value_abs <= 15) cat = '4';
    else if (value_abs <= 31) cat = '5';
    else if (value_abs <= 63) cat = '6';
    else if (value_abs <= 127) cat = '7';
    else if (value_abs <= 255) cat = '8';
    else if (value_abs <= 511) cat = '9';
    else if (value_abs <= 1023) cat = 'A';
    else cat='A';

    // com a cat, acho o prefixo
    int qtd_pref;
    uint32_t pref = get_prefixoAC(qtd_zero, cat, &qtd_pref);

    // pego o complemento de 1 do meu valor
    int qtd_v; 
    uint32_t mant = get_mantissa_comp1(value, &qtd_v);

    // junto tudo em um valor
    uint32_t res = shifta_e_grava(pref, mant, qtd_v);

    // seto a qtd de bits validos em res
    *qtd_bits = qtd_pref+qtd_v;

    // retorno
    return res;
}


