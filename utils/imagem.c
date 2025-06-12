#include "imagem.h"
#include "bloco.h"
#include "gravador.h"
#include "codifica.h"
#include "decodifica.h"
#include "leitor.h"

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

struct imagem_ {
    BMPFILEHEADER* fileHeader;
    BMPINFOHEADER* infoHeader;
    int h, w;
    int cbcr_h, cbcr_w;
    unsigned char **r, **g, **b;
    double **y, **cb, **cr;
};

IMAGEM* criarImagem(FILE* f) {
    // cria imagem
    IMAGEM* img = (IMAGEM*) malloc(sizeof(IMAGEM));

    // pega o cabecalho
    img->fileHeader = leituraFileHeader(f);
    img->infoHeader = leituraInfoHeader(f);

    // pega as dimensoes no cabecalho
    img->w = get_biWidth(img->infoHeader);
    img->h = get_biHeight(img->infoHeader);

    int qtd_pixels = img->w*img->h;

    // aloca matrizes r,g,b
    alocarMatriz_unsignedChar(&(img->r), img->h, img->w);
    alocarMatriz_unsignedChar(&(img->g), img->h, img->w);
    alocarMatriz_unsignedChar(&(img->b), img->h, img->w);

    // aloca luminancia
    alocarMatriz_double(&(img->y), img->h, img->w);

    // le r,g,b e ja calcula luminancia
    for(int i = 0; i < img->h; i++) {
        for(int j = 0; j < img->w; j++) {
            fread(&((img->b)[i][j]), sizeof(unsigned char), 1, f);
            fread(&((img->g)[i][j]), sizeof(unsigned char), 1, f);
            fread(&((img->r)[i][j]), sizeof(unsigned char), 1, f);

            (img->y)[i][j] = 0.299*(double)(img->r)[i][j] + 0.587*(double)(img->g)[i][j] + 0.114*(double)(img->b)[i][j];
        }
    }

    // calcula as dimensoes que cb,cr vao ter
    // e "corrige" para ser multiplo de 8
    img->cbcr_w = (img->w)/2 + (8 - ((img->w)/2)%8);
    img->cbcr_h = (img->h)/2 + (8 - ((img->h)/2)%8);

    // aloca essas matrizes
    alocarMatriz_double(&(img->cb), img->cbcr_w, img->cbcr_h);
    alocarMatriz_double(&(img->cr), img->cbcr_w, img->cbcr_h);

    int new_i = 0;

    // calcula cb,cr (faz o downsampling)
    for(int i = 0; i < (img->h); i+=2) {
        int new_j = 0;
        for(int j = 0; j < (img->w); j+=2) {
            double soma_bloco_cb = ((double)(img->b)[i][j]-(img->y)[i][j]) + ((double)(img->b)[i+1][j]-(img->y)[i+1][j]) + ((double)(img->b)[i+1][j+1]-(img->y)[i+1][j+1]) + ((double)(img->b)[i][j+1]-(img->y)[i][j+1]);
            double soma_bloco_cr = ((double)(img->r)[i][j]-(img->y)[i][j]) + ((double)(img->r)[i+1][j]-(img->y)[i+1][j]) + ((double)(img->r)[i+1][j+1]-(img->y)[i+1][j+1]) + ((double)(img->r)[i][j+1]-(img->y)[i][j+1]);

            (img->cb)[new_i][new_j] = ((0.564*soma_bloco_cb)/4.0) + 128.0;
            (img->cr)[new_i][new_j] = ((0.713*soma_bloco_cr)/4.0) + 128.0;

            new_j++;
        }
        new_i++;
    }


    // agora copia linhas/coluna para preencher 
    // o espaço que falta (pq precisa ser mult de 8)

    int old_w = (img->w)/2;
    int old_h = (img->h)/2;

    // conserta coluna
    for(int k = 0; k < old_h; k++) {
        double value_cb = (img->cb)[k][old_w-1];
        double value_cr = (img->cr)[k][old_w-1];

        for(int j = old_w; j < img->cbcr_w; j++) {
            (img->cb)[k][j] = value_cb;
            (img->cr)[k][j] = value_cr;
        }
    }

    // conserta linha
    for(int k = 0; k < img->cbcr_w; k++){
        double value_cb = (img->cb)[old_h-1][k];
        double value_cr = (img->cr)[old_h-1][k];

        for(int j = old_h; j < img->cbcr_h; j++){
            (img->cb)[j][k] = value_cb;
            (img->cr)[j][k] = value_cr;
        }
    }

    // Debug.
    //BLOCO* b = criarBloco(img->y, 0, 0, 'L');
    //printf("Primeiro bloco Y da entrada:\n");
    //printBloco(b);

    //printFileHeader(img->fileHeader);
    //printInfoHeader(img->infoHeader);


    return img;
}

void codifica_gravaVetor(GRAVADOR* gravador, int* vetor, int valor_difDC) {
    // grava DC
    int qtd_DC;
    uint32_t codificaDC = codifica_infoDC(valor_difDC, &qtd_DC);
    gravarValor(gravador, codificaDC, qtd_DC);

    int qtd_zeros = 0;

    for(int i = 1; i < 64; i++) {
        if (vetor[i] == 0) {
            qtd_zeros++;
        } else {
            int valor = vetor[i];
            int qtd_AC;
            uint32_t codificaAC = codifica_infoAC(qtd_zeros, valor, &qtd_AC);
            gravarValor(gravador, codificaAC, qtd_AC);

            qtd_zeros = 0;
        }
    }

    int qtd_fim;
    uint32_t codifica_fim;

    if (qtd_zeros == 0) {
        codifica_fim = codifica_infoAC(0, 0, &qtd_fim);
    } else {
        codifica_fim = codifica_infoAC(-1, 0, &qtd_fim);
    }

    gravarValor(gravador, codifica_fim, qtd_fim);
}

void comprimeImagem(IMAGEM* img, FILE* f) {
    escreverFileHeader(f, img->fileHeader);
    escreverInfoHeader(f, img->infoHeader);

    GRAVADOR* gravador = criarGravador(f);

    int ultimo_dc = 0;

    for(int i = 0; i < (img->h); i += 8) {
        for(int j = 0; j < (img->w); j += 8) {
            BLOCO* bloco1 = criarBloco(img->y, i, j, 'L');
            BLOCO* bloco1_dct = aplicaDCT(bloco1);
            BLOCO* bloco1_qtz = aplicaQuantizacao(bloco1_dct);
            int* vetor_final = pega_zigzag(bloco1_qtz);
            codifica_gravaVetor(gravador, vetor_final, vetor_final[0]-ultimo_dc);
            ultimo_dc = vetor_final[0];
        }
    }

    ultimo_dc = 0;

    for(int i = 0; i < img->cbcr_h; i += 8) {
        for(int j = 0; j < img->cbcr_w; j += 8) {
            BLOCO* bloco1 = criarBloco(img->cb, i, j, 'B');
            BLOCO* bloco1_dct = aplicaDCT(bloco1);
            BLOCO* bloco1_qtz = aplicaQuantizacao(bloco1_dct);
            int* vetor_final = pega_zigzag(bloco1_qtz);
            codifica_gravaVetor(gravador, vetor_final, vetor_final[0]-ultimo_dc);
            ultimo_dc = vetor_final[0];
        }
    }

    ultimo_dc = 0;

    for(int i = 0; i < img->cbcr_h; i += 8) {
        for(int j = 0; j < img->cbcr_w; j += 8) {
            BLOCO* bloco1 = criarBloco(img->cr, i, j, 'R');
            BLOCO* bloco1_dct = aplicaDCT(bloco1);
            BLOCO* bloco1_qtz = aplicaQuantizacao(bloco1_dct);
            int* vetor_final = pega_zigzag(bloco1_qtz);
            codifica_gravaVetor(gravador, vetor_final, vetor_final[0]-ultimo_dc);
            ultimo_dc = vetor_final[0];
        }
    }

    finalizarGravacao(gravador);
}

void escreverImagem(FILE* f, IMAGEM* img) {
    escreverFileHeader(f, img->fileHeader);
    escreverInfoHeader(f, img->infoHeader);

    double menor = 0.0;
    double maior = 0.0;

    for(int i = 0; i < img->h; i++) {
        for(int j = 0; j < img->w; j++) {
            double cr = (img->cr)[i/2][j/2] - 128.0;
            double cb = (img->cb)[i/2][j/2] - 128.0;

            double r_ = (img->y)[i][j] + 1.402*cr;
            double g_ = (img->y)[i][j] - 0.344*cb - 0.714*cr;
            double b_ = (img->y)[i][j] + 1.772*cb;

            unsigned char r = (unsigned char) max(0, min(255, r_));
            unsigned char g = (unsigned char) max(0, min(255, g_));
            unsigned char b = (unsigned char) max(0, min(255, b_));

            fwrite(&b, sizeof(unsigned char), 1, f);
            fwrite(&g, sizeof(unsigned char), 1, f);
            fwrite(&r, sizeof(unsigned char), 1, f);
        }
    }

    //printf("%lf / %lf\n", menor, maior);
}

// Função para debug.
void printarImagem(IMAGEM* img) {
    printf("====IMPRESSÃO DA IMAGEM====\n%d %d %d %d\n", img->h, img->w, img->cbcr_h, img->cbcr_w);

    printFileHeader(img->fileHeader);
    printInfoHeader(img->infoHeader);

    for(int i = 0; i < img->h; i++){
        for(int j = 0; j < img->w; j++){
            printf("%4d %.4d %4d\n", (img->r)[i][j], (img->g)[i][j], (img->b)[i][j]);
        }
        printf("\n");
    }

    return;
}


IMAGEM* descomprimeImagem(FILE* in, FILE* out){
    iniciarDecodificacao();
    //printf("\n\n============ NA FUNÇÃO DESCOMPRIMEIMAGEM ============\n\n");
    // Lê o cabeçalho e os dados adicionais e armazena as dimensões da imagem original em h e w.
    IMAGEM* img = malloc(sizeof(IMAGEM));

    img->fileHeader = leituraFileHeader(in);
    img->infoHeader = leituraInfoHeader(in);

    // Debug.
    //printFileHeader(img->fileHeader);
    //printInfoHeader(img->infoHeader);

    // pega as dimensoes no cabecalho
    img->w = get_biWidth(img->infoHeader);
    img->h = get_biHeight(img->infoHeader);

    // Número de blocos 8x8 da componente Y.
    int num_blocos_Y = (img->h/8)*(img->w/8);

    int** blocos_em_vetor_Y = (int**) malloc(sizeof(int*)*num_blocos_Y);
    for(int i = 0; i < num_blocos_Y; i++){
        blocos_em_vetor_Y[i] = (int*) malloc(sizeof(int)*64);
    }

    // h/2 e w/2 podem não ser múltiplos de 8. Se não for, tem que somar 1 a (h/2)/8 e (w/2)/8 pra calcular número de blocos.
    int bloco_hExtra = ((img->h/2) % 8) ? 1 : 0;
    int bloco_wExtra = ((img->w/2) % 8) ? 1 : 0;
    int num_blocos_CbCr = (img->h/2/8 + bloco_hExtra)*(img->w/2/8 + bloco_wExtra);
    // Tamanho das matrizes Cb e Cr.
    img->cbcr_h = (img->h/2) + 8*bloco_hExtra;
    img->cbcr_w = (img->w/2) + 8*bloco_wExtra;

    //printf("h: %d w: %d cbcr_h: %d cbcr_w: %d\n\n", img->h, img->w, img->cbcr_h, img->cbcr_w);
    //printf("Número de blocos Y: %d Número de blocos CbCr: %d\n\n", num_blocos_Y, num_blocos_CbCr);


    int** blocos_em_vetor_Cb = (int**) malloc(sizeof(int*)*num_blocos_CbCr);
    int** blocos_em_vetor_Cr = (int**) malloc(sizeof(int*)*num_blocos_CbCr);
    for(int i = 0; i < num_blocos_CbCr; i++){
        blocos_em_vetor_Cb[i] = (int*) malloc(sizeof(int)*64);
        blocos_em_vetor_Cr[i] = (int*) malloc(sizeof(int)*64);
    }

    // Debug.
    //printf("Dimensões da imagem:\nh: %d w: %d\nnumBlocosCbCr: %d numBlocosY: %d\n\n", img->h, img->w, num_blocos_CbCr, num_blocos_Y);

    LEITOR* leitor = criarLeitor(in);
    int DC_anterior = 0;
    int ehPrimeiroDC = 1;
    for(int i = 0; i < num_blocos_Y; i++){
        decodificaDC(&(blocos_em_vetor_Y[i][0]), leitor, DC_anterior, &ehPrimeiroDC);
        DC_anterior = blocos_em_vetor_Y[i][0];

        decodificaAC(blocos_em_vetor_Y[i], leitor);


        //printf("=============== BLOCO %d ==============\n", i);
        //for(int j = 0; j < 64; j++){
        //    printf("%d ", blocos_em_vetor_Y[i][j]);
        //}
        //printf("\n");
    }

    ehPrimeiroDC = 1;
    DC_anterior = 0;
    for(int i = 0; i < num_blocos_CbCr; i++){
        decodificaDC(&(blocos_em_vetor_Cb[i][0]), leitor, DC_anterior, &ehPrimeiroDC);
        DC_anterior = blocos_em_vetor_Cb[i][0];

        decodificaAC(blocos_em_vetor_Cb[i], leitor);
        
        //for(int j = 0; j < 64; j++){
        //    printf("%d ", blocos_em_vetor_Cb[i][j]);
        //}
        //printf("\n");
    }

    ehPrimeiroDC = 1;
    DC_anterior = 0;
    for(int i = 0; i < num_blocos_CbCr; i++){
        decodificaDC(&(blocos_em_vetor_Cr[i][0]), leitor, DC_anterior, &ehPrimeiroDC);
        DC_anterior = blocos_em_vetor_Cr[i][0];

        decodificaAC(blocos_em_vetor_Cr[i], leitor);
        
        //for(int j = 0; j < 64; j++){
        //    printf("%d ", blocos_em_vetor_Cr[i][j]);
        //}
        //printf("\n");
    }

    alocarMatriz_unsignedChar(&(img->r), img->h, img->w);
    alocarMatriz_unsignedChar(&(img->g), img->h, img->w);
    alocarMatriz_unsignedChar(&(img->b), img->h, img->w);

    alocarMatriz_double(&(img->y), img->h, img->w);
    alocarMatriz_double(&(img->cb), img->cbcr_h, img->cbcr_w);
    alocarMatriz_double(&(img->cr), img->cbcr_h, img->cbcr_w);

    BLOCO *bloco, *quantizacao_inversa, *DCT_inversa;
    for(int i = 0; i < num_blocos_Y; i++){
        //printf("loop %d\n", i);
        //printf("número do bloco: %d\n\n", i);
        bloco = monta_bloco(blocos_em_vetor_Y[i], 'L');
        //if(i == 0){
        //    printf("Primeiro bloco Y:\n");
        //    printBloco(bloco);
        //}
        quantizacao_inversa = desfazQuantizacao(bloco);
        //if(i == 0){
        //    printf("Primeiro bloco Y desquantizado:\n");
        //    printBloco(quantizacao_inversa);
        //}
        DCT_inversa = desfazDCT(quantizacao_inversa);
        gravaBloco(img->y, 8*((i*8)/img->w), (i*8)%img->w, DCT_inversa);
        //printf("[%d][%d]\n", 8*((i*8)/img->w), (i*8)%img->w);

        //if(i == 0){
        //    printf("Primeiro bloco Y com DCT desfeita:\n");
        //    printBloco(DCT_inversa);
        //}

        desalocarBloco(&bloco);
        desalocarBloco(&quantizacao_inversa);
        desalocarBloco(&DCT_inversa);
    }

    for(int i = 0; i < num_blocos_CbCr; i++){
        bloco = monta_bloco(blocos_em_vetor_Cb[i], 'B');
        quantizacao_inversa = desfazQuantizacao(bloco);
        DCT_inversa = desfazDCT(quantizacao_inversa);
        gravaBloco(img->cb, 8*((i*8)/img->cbcr_w), (i*8)%img->cbcr_w, DCT_inversa);
        //printf("[%d][%d]\n", 8*((i*8)/img->cbcr_w), (i*8)%img->cbcr_w);

        desalocarBloco(&bloco);
        desalocarBloco(&quantizacao_inversa);
        desalocarBloco(&DCT_inversa);
    }

    for(int i = 0; i < num_blocos_CbCr; i++){
        //printf("loop %d\n", i);
        bloco = monta_bloco(blocos_em_vetor_Cr[i], 'R');
        quantizacao_inversa = desfazQuantizacao(bloco);
        DCT_inversa = desfazDCT(quantizacao_inversa);
        gravaBloco(img->cr, 8*((i*8)/img->cbcr_w), (i*8)%img->cbcr_w, DCT_inversa);
        //printf("[%d][%d]\n", 8*((i*8)/img->cbcr_w), (i*8)%img->cbcr_w);

        desalocarBloco(&bloco);
        desalocarBloco(&quantizacao_inversa);
        desalocarBloco(&DCT_inversa);
    }

    // Transformação para RGB.
    int rgb_i = 0, rgb_j = 0;
    for(int i = 0; i < img->h/2; i++){
        rgb_j = 0;
        double constante;
        for(int j = 0; j < img->w/2; j++){
            constante = (img->cr)[i][j] - 128;
            (img->r)[rgb_i][rgb_j] = (constante)*1.402 + (img->y)[rgb_i][rgb_j];
            (img->r)[rgb_i][rgb_j+1] = (constante)*1.402 + (img->y)[rgb_i][rgb_j+1];
            (img->r)[rgb_i+1][rgb_j] = (constante)*1.402 + (img->y)[rgb_i+1][rgb_j];
            (img->r)[rgb_i+1][rgb_j+1] = (constante)*1.402 + (img->y)[rgb_i+1][rgb_j+1];

            constante = (img->cb)[i][j] - 128;
            (img->b)[rgb_i][rgb_j] = (constante)*1.772 + (img->y)[rgb_i][rgb_j];
            (img->b)[rgb_i][rgb_j+1] = (constante)*1.772 + (img->y)[rgb_i][rgb_j+1];
            (img->b)[rgb_i+1][rgb_j] = (constante)*1.772 + (img->y)[rgb_i+1][rgb_j];
            (img->b)[rgb_i+1][rgb_j+1] = (constante)*1.772 + (img->y)[rgb_i+1][rgb_j+1];

            constante = -0.344136*((img->cb)[i][j] - 128) - 0.714136*((img->cr)[i][j] - 128);
            (img->g)[rgb_i][rgb_j] = (img->y)[rgb_i][rgb_j] + constante;
            (img->g)[rgb_i][rgb_j+1] = (img->y)[rgb_i][rgb_j+1] + constante;
            (img->g)[rgb_i+1][rgb_j] = (img->y)[rgb_i+1][rgb_j] + constante;
            (img->g)[rgb_i+1][rgb_j+1] = (img->y)[rgb_i+1][rgb_j+1] + constante;

            rgb_j += 2;
        }
        rgb_i += 2;
    }

    // Debug.

    //printf("\n\n========== MATRIZ R ==========\n\n");
    //printMatrizUchar(img->r, img->h, img->w);
    //printf("\n\n========== MATRIZ G ==========\n\n");
    //printMatrizUchar(img->g, img->h, img->w);
    //printf("\n\n========== MATRIZ B ==========\n\n");
    //printMatrizUchar(img->b, img->h, img->w);

    //printf("\n\n========== MATRIZ Y ==========\n\n");
    //printMatrizDouble(img->y, img->h, img->w);
    //printf("\n\n========== MATRIZ CB ==========\n\n");
    //printMatrizDouble(img->cb, img->cbcr_h, img->cbcr_w);
    //printf("\n\n========== MATRIZ CR ==========\n\n");
    //printMatrizDouble(img->cr, img->cbcr_h, img->cbcr_w);

    encerrarDecodificacao();
    destruirLeitor(&leitor);

    desalocarMatriz_int(&blocos_em_vetor_Y, num_blocos_Y, 64);
    desalocarMatriz_int(&blocos_em_vetor_Cb, num_blocos_CbCr, 64);
    desalocarMatriz_int(&blocos_em_vetor_Cr, num_blocos_CbCr, 64);

    return img;
}

void salvarBMP(FILE* f, IMAGEM* img) {
    // Escreve cabeçalhos
    fseek(f, 0, SEEK_SET);
    escreverFileHeader(f, img->fileHeader);
    escreverInfoHeader(f, img->infoHeader);

    int largura = img->w;
    int altura = img->h;

    for (int i = 0; i < altura; i++) { // BMP escreve de baixo pra cima
        for (int j = 0; j < largura; j++) {
            unsigned char pixel[3];
            pixel[0] = img->b[i][j]; // Azul
            pixel[1] = img->g[i][j]; // Verde
            pixel[2] = img->r[i][j]; // Vermelho
            fwrite(pixel, 1, 3, f);
        }
    }
}

// Funções de debug.
void printarInt(int byte){
    for(int i = 31; i >= 0; i--){
        if(byte & (1 << i))
            printf("1");
        else
            printf("0");
    }
    printf("\n");
}

void printarUint_32(uint32_t buffer){
    for(int i = sizeof(uint32_t)*8 - 1; i >= 0; i--){
        if(buffer & (1 << i))
            printf("1");
        else
            printf("0");
    }
    printf("\n");
}

void printarLongInt(long int byte){
    for(int i = 63; i >= 0; i--){
        if(byte & (1 << i))
            printf("1");
        else
            printf("0");
    }
    printf("\n");
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

void desalocarImagem(IMAGEM** img) {
    desalocarFileHeader(&((*img)->fileHeader));
    desalocarInfoHeader(&((*img)->infoHeader));

    desalocarMatriz_unsignedChar(&((*img)->r), (*img)->h, (*img)->w);
    desalocarMatriz_unsignedChar(&((*img)->g), (*img)->h, (*img)->w);
    desalocarMatriz_unsignedChar(&((*img)->b), (*img)->h, (*img)->w);

    desalocarMatriz_double(&((*img)->y), (*img)->h, (*img)->w);
    desalocarMatriz_double(&((*img)->cb), (*img)->cbcr_h, (*img)->cbcr_w);
    desalocarMatriz_double(&((*img)->cr), (*img)->cbcr_h, (*img)->cbcr_w);

    free(*img);

    *img = NULL; 
}