#include "codifica.h"
#include "util.h"

uint32_t codifica_infoDC(int value, int* qtd_bits) {
    int value_abs = abs(value);
    char cat = 'X';

    // pega a categoria - suponho q no max value <= 1023
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

    uint32_t res = 0;
    int qtd = 0;

    // guarda o prefixo
    switch (cat)
    {
    case '0':
        res = shifta_e_grava(res, 2, 3);
        qtd = 3;
        break;
    
    case '1':
        res = shifta_e_grava(res, 3, 3);
        qtd = 3;
        break;
    
    case '2':
        res = shifta_e_grava(res, 4, 3);
        qtd = 3;
        break;
    
    case '3':
        res = shifta_e_grava(res, 0, 2);
        qtd = 2;
        break;
    
    case '4':
        res = shifta_e_grava(res, 5, 3);
        qtd = 3;
        break;

    case '5':
        res = shifta_e_grava(res, 6, 3);
        qtd = 3;
        break;
    
    case '6':
        res = shifta_e_grava(res, 14, 4);
        qtd = 4;
        break;
    
    case '7':
        res = shifta_e_grava(res, 30, 5);
        qtd = 5;
        break;
    
    case '8':
        res = shifta_e_grava(res, 62, 6);
        qtd = 6;
        break;
    
    case '9':
        res = shifta_e_grava(res, 126, 7);
        qtd = 7;
        break;
    
    case 'A':
        res = shifta_e_grava(res, 254, 8);
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

uint32_t tabelaAC[16][10];
void inicializaTabelaAC();
int iniciou = 0;

uint32_t get_prefixoAC(int zeros, char cat, int* qtd_bits) {
    if (!iniciou) {
        inicializaTabelaAC();
        iniciou = 1;
    }

    if (cat == '0') {
        // nesse caso, só tem 0 até o fim
        if (zeros == 15) {
            *qtd_bits = 12;
            return 0b111111110111;
        } else if (zeros == 0) {
            *qtd_bits = 4;
            return 0b1010;
        }
    } 

    int j;

    if (cat == 'A') {
        j = 9;
    } else {
        j = (int) ((int)cat-(int)'0');
    }

    uint32_t pref = tabelaAC[zeros][j-1];

    *qtd_bits = get_qtdBits(pref);

    if (zeros == 0 && (cat=='1' || cat=='2')) {
        *qtd_bits += 1;
        if (cat=='1') *qtd_bits += 1;
    }

    return pref;
}


uint32_t codifica_infoAC(int qtd_zero, int value, int* qtd_bits) {
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

    int qtd_pref;
    uint32_t pref = get_prefixoAC(qtd_zero, cat, &qtd_pref);

    int qtd_v; 
    uint32_t mant = get_mantissa_comp1(value, &qtd_v);

    uint32_t res = shifta_e_grava(pref, mant, qtd_v);

    *qtd_bits = qtd_pref+qtd_v;
    return res;
}


void inicializaTabelaAC() {
    tabelaAC[0][0] = 0b00;
    tabelaAC[0][1] = 0b01;
    tabelaAC[0][2] = 0b100;
    tabelaAC[0][3] = 0b1011;
    tabelaAC[0][4] = 0b11010;
    tabelaAC[0][5] = 0b111000;
    tabelaAC[0][6] = 0b1111000;
    tabelaAC[0][7] = 0b1111110110;
    tabelaAC[0][8] = 0b1111111110000010;
    tabelaAC[0][9] = 0b1111111110000011;

    tabelaAC[1][0] = 0b1100;
    tabelaAC[1][1] = 0b111001;
    tabelaAC[1][2] = 0b1111001;
    tabelaAC[1][3] = 0b111110110;
    tabelaAC[1][4] = 0b11111110110;
    tabelaAC[1][5] = 0b1111111110000100;
    tabelaAC[1][6] = 0b1111111110000101;
    tabelaAC[1][7] = 0b1111111110000110;
    tabelaAC[1][8] = 0b1111111110000111;
    tabelaAC[1][9] = 0b1111111110001000;

    tabelaAC[2][0] = 0b11011;
    tabelaAC[2][1] = 0b11111000;
    tabelaAC[2][2] = 0b1111110111;
    tabelaAC[2][3] = 0b1111111110001001;
    tabelaAC[2][4] = 0b1111111110001010;
    tabelaAC[2][5] = 0b1111111110001011;
    tabelaAC[2][6] = 0b1111111110001100;
    tabelaAC[2][7] = 0b1111111110001101;
    tabelaAC[2][8] = 0b1111111110001110;
    tabelaAC[2][9] = 0b1111111110001111;

    tabelaAC[3][0] = 0b111010;
    tabelaAC[3][1] = 0b111110111;
    tabelaAC[3][2] = 0b11111110111;
    tabelaAC[3][3] = 0b1111111110010000;
    tabelaAC[3][4] = 0b1111111110010001;
    tabelaAC[3][5] = 0b1111111110010010;
    tabelaAC[3][6] = 0b1111111110010011;
    tabelaAC[3][7] = 0b1111111110010100;
    tabelaAC[3][8] = 0b1111111110010101;
    tabelaAC[3][9] = 0b1111111110010110;

    tabelaAC[4][0] = 0b111011;
    tabelaAC[4][1] = 0b1111111000;
    tabelaAC[4][2] = 0b1111111110010111;
    tabelaAC[4][3] = 0b1111111110011000;
    tabelaAC[4][4] = 0b1111111110011001;
    tabelaAC[4][5] = 0b1111111110011010;
    tabelaAC[4][6] = 0b1111111110011011;
    tabelaAC[4][7] = 0b1111111110011100;
    tabelaAC[4][8] = 0b1111111110011101;
    tabelaAC[4][9] = 0b1111111110011110;

    tabelaAC[5][0] = 0b1111010;
    tabelaAC[5][1] = 0b1111111001;
    tabelaAC[5][2] = 0b1111111110011111;
    tabelaAC[5][3] = 0b1111111110100000;
    tabelaAC[5][4] = 0b1111111110100001;
    tabelaAC[5][5] = 0b1111111110100010;
    tabelaAC[5][6] = 0b1111111110100011;
    tabelaAC[5][7] = 0b1111111110100100;
    tabelaAC[5][8] = 0b1111111110100101;
    tabelaAC[5][9] = 0b1111111110100110;

    tabelaAC[6][0] = 0b1111011;
    tabelaAC[6][1] = 0b11111111000;
    tabelaAC[6][2] = 0b1111111110100111;
    tabelaAC[6][3] = 0b1111111110101000;
    tabelaAC[6][4] = 0b1111111110101001;
    tabelaAC[6][5] = 0b1111111110101010;
    tabelaAC[6][6] = 0b1111111110101011;
    tabelaAC[6][7] = 0b1111111110101100;
    tabelaAC[6][8] = 0b1111111110101101;
    tabelaAC[6][9] = 0b1111111110101110;

    tabelaAC[7][0] = 0b11111001;
    tabelaAC[7][1] = 0b11111111001;
    tabelaAC[7][2] = 0b1111111110101111;
    tabelaAC[7][3] = 0b1111111110110000;
    tabelaAC[7][4] = 0b1111111110110001;
    tabelaAC[7][5] = 0b1111111110110010;
    tabelaAC[7][6] = 0b1111111110110011;
    tabelaAC[7][7] = 0b1111111110110100;
    tabelaAC[7][8] = 0b1111111110110101;
    tabelaAC[7][9] = 0b1111111110110110;

    tabelaAC[8][0] = 0b11111010;
    tabelaAC[8][1] = 0b111111111000000;
    tabelaAC[8][2] = 0b1111111110110111;
    tabelaAC[8][3] = 0b1111111110111000;
    tabelaAC[8][4] = 0b1111111110111001;
    tabelaAC[8][5] = 0b1111111110111010;
    tabelaAC[8][6] = 0b1111111110111011;
    tabelaAC[8][7] = 0b1111111110111100;
    tabelaAC[8][8] = 0b1111111110111101;
    tabelaAC[8][9] = 0b1111111110111110;

    tabelaAC[9][0] = 0b111111000;
    tabelaAC[9][1] = 0b1111111110111111;
    tabelaAC[9][2] = 0b1111111111000000;
    tabelaAC[9][3] = 0b1111111111000001;
    tabelaAC[9][4] = 0b1111111111000010;
    tabelaAC[9][5] = 0b1111111111000011;
    tabelaAC[9][6] = 0b1111111111000100;
    tabelaAC[9][7] = 0b1111111111000101;
    tabelaAC[9][8] = 0b1111111111000110;
    tabelaAC[9][9] = 0b1111111111000111;

    tabelaAC[10][0] = 0b111111001;
    tabelaAC[10][1] = 0b1111111111001000;
    tabelaAC[10][2] = 0b1111111111001001;
    tabelaAC[10][3] = 0b1111111111001010;
    tabelaAC[10][4] = 0b1111111111001011;
    tabelaAC[10][5] = 0b1111111111001100;
    tabelaAC[10][6] = 0b1111111111001101;
    tabelaAC[10][7] = 0b1111111111001110;
    tabelaAC[10][8] = 0b1111111111001111;
    tabelaAC[10][9] = 0b1111111111010000;

    tabelaAC[11][0] = 0b111111010;
    tabelaAC[11][1] = 0b1111111111010001;
    tabelaAC[11][2] = 0b1111111111010010;
    tabelaAC[11][3] = 0b1111111111010011;
    tabelaAC[11][4] = 0b1111111111010100;
    tabelaAC[11][5] = 0b1111111111010101;
    tabelaAC[11][6] = 0b1111111111010110;
    tabelaAC[11][7] = 0b1111111111010111;
    tabelaAC[11][8] = 0b1111111111011000;
    tabelaAC[11][9] = 0b1111111111011001;

    tabelaAC[12][0] = 0b1111111010;
    tabelaAC[12][1] = 0b1111111111011010;
    tabelaAC[12][2] = 0b1111111111011011;
    tabelaAC[12][3] = 0b1111111111011100;
    tabelaAC[12][4] = 0b1111111111011101;
    tabelaAC[12][5] = 0b1111111111011110;
    tabelaAC[12][6] = 0b1111111111011111;
    tabelaAC[12][7] = 0b1111111111100000;
    tabelaAC[12][8] = 0b1111111111100001;
    tabelaAC[12][9] = 0b1111111111100010;

    tabelaAC[13][0] = 0b11111111010;
    tabelaAC[13][1] = 0b1111111111100011;
    tabelaAC[13][2] = 0b1111111111100100;
    tabelaAC[13][3] = 0b1111111111100101;
    tabelaAC[13][4] = 0b1111111111100110;
    tabelaAC[13][5] = 0b1111111111100111;
    tabelaAC[13][6] = 0b1111111111101000;
    tabelaAC[13][7] = 0b1111111111101001;
    tabelaAC[13][8] = 0b1111111111101010;
    tabelaAC[13][9] = 0b1111111111101011;

    tabelaAC[14][0] = 0b111111110110;
    tabelaAC[14][1] = 0b1111111111101100;
    tabelaAC[14][2] = 0b1111111111101101;
    tabelaAC[14][3] = 0b1111111111101110;
    tabelaAC[14][4] = 0b1111111111101111;
    tabelaAC[14][5] = 0b1111111111110000;
    tabelaAC[14][6] = 0b1111111111110001;
    tabelaAC[14][7] = 0b1111111111110010;
    tabelaAC[14][8] = 0b1111111111110011;
    tabelaAC[14][9] = 0b1111111111110100;

    tabelaAC[15][0] = 0b1111111111110101;
    tabelaAC[15][1] = 0b1111111111110110;
    tabelaAC[15][2] = 0b1111111111110111;
    tabelaAC[15][3] = 0b1111111111111000;
    tabelaAC[15][4] = 0b1111111111111001;
    tabelaAC[15][5] = 0b1111111111111010;
    tabelaAC[15][6] = 0b1111111111111011;
    tabelaAC[15][7] = 0b1111111111111100;
    tabelaAC[15][8] = 0b1111111111111101;
    tabelaAC[15][9] = 0b1111111111111110;
}

