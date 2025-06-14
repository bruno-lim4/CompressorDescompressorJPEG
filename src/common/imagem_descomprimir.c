#include "imagem_descomprimir.h"
#include "bloco.h"
#include "../descompressor/leitor.h"

struct imagem_ {
    BMPFILEHEADER* fileHeader;
    BMPINFOHEADER* infoHeader;
    int h, w;
    int cbcr_h, cbcr_w;
    unsigned char **r, **g, **b;
    double **y, **cb, **cr;
};

IMAGEM* descomprimeImagem(FILE* in){
    iniciarDecodificacao(); // Inicializa árvores de prefixos.
    IMAGEM* img = malloc(sizeof(IMAGEM));

    img->fileHeader = leituraFileHeader(in);
    img->infoHeader = leituraInfoHeader(in);

    // pega as dimensoes no cabecalho
    img->w = get_biWidth(img->infoHeader);
    img->h = get_biHeight(img->infoHeader);

    printf("dimensao = (%d, %d)", img->h, img->w);

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
    // Tamanho das matrizes Cb e Cr; 8 * número de blocos.
    img->cbcr_h = 8*(img->h/2/8 + bloco_hExtra);
    img->cbcr_w = 8*(img->w/2/8 + bloco_wExtra);

    printf("cbcr = (%d, %d)\n", img->cbcr_h, img->cbcr_w);

    int** blocos_em_vetor_Cb = (int**) malloc(sizeof(int*)*num_blocos_CbCr);
    int** blocos_em_vetor_Cr = (int**) malloc(sizeof(int*)*num_blocos_CbCr);
    for(int i = 0; i < num_blocos_CbCr; i++){
        blocos_em_vetor_Cb[i] = (int*) malloc(sizeof(int)*64);
        blocos_em_vetor_Cr[i] = (int*) malloc(sizeof(int)*64);
    }

    LEITOR* leitor = criarLeitor(in); // Estrutura pra ler um bit por vez.
    int DC_anterior = 0; // Pra somar com a diferença decodificada e achar o próximo DC.
    int ehPrimeiroDC = 1; // Flag pra sinalizar se é o primeiro DC a ser decodificado.
    for(int i = 0; i < num_blocos_Y; i++){
        decodificaDC(&(blocos_em_vetor_Y[i][0]), leitor, DC_anterior, &ehPrimeiroDC);
        DC_anterior = blocos_em_vetor_Y[i][0];

        decodificaAC(blocos_em_vetor_Y[i], leitor);

        printf("vetor de 64 pos: ");
        for(int j = 0; j < 64; j++){
            printf(" %d", blocos_em_vetor_Y[i][j]);
        }
        printf("\n\n");
    }

    ehPrimeiroDC = 1;
    DC_anterior = 0;
    for(int i = 0; i < num_blocos_CbCr; i++){
        decodificaDC(&(blocos_em_vetor_Cb[i][0]), leitor, DC_anterior, &ehPrimeiroDC);
        DC_anterior = blocos_em_vetor_Cb[i][0];

        decodificaAC(blocos_em_vetor_Cb[i], leitor);

        printf("vetor de 64 pos: ");
        for(int j = 0; j < 64; j++){
            printf(" %d", blocos_em_vetor_Cb[i][j]);
        }
        printf("\n\n");

    }

    ehPrimeiroDC = 1;
    DC_anterior = 0;
    printf("numblocos = %d\n\n", num_blocos_CbCr);
    for(int i = 0; i < num_blocos_CbCr; i++){
        //printf("i = %d\n", i);
        decodificaDC(&(blocos_em_vetor_Cr[i][0]), leitor, DC_anterior, &ehPrimeiroDC);
        DC_anterior = blocos_em_vetor_Cr[i][0];

        decodificaAC(blocos_em_vetor_Cr[i], leitor);

        printf("vetor de 64 pos: ");
        for(int j = 0; j < 64; j++){
            printf(" %d", blocos_em_vetor_Cr[i][j]);
        }
        printf("\n\n");
    }
    exit(1);

    alocarMatriz_unsignedChar(&(img->r), img->h, img->w);
    alocarMatriz_unsignedChar(&(img->g), img->h, img->w);
    alocarMatriz_unsignedChar(&(img->b), img->h, img->w);

    alocarMatriz_double(&(img->y), img->h, img->w);
    alocarMatriz_double(&(img->cb), img->cbcr_h, img->cbcr_w);
    alocarMatriz_double(&(img->cr), img->cbcr_h, img->cbcr_w);

    BLOCO *bloco, *quantizacao_inversa, *DCT_inversa;
    // Reconstrói os blocos e desfaz quantização e DCT.
    // Componente Y.
    for(int i = 0; i < num_blocos_Y; i++){
        bloco = monta_bloco(blocos_em_vetor_Y[i], 'L');

        quantizacao_inversa = desfazQuantizacao(bloco);

        DCT_inversa = desfazDCT(quantizacao_inversa);
        gravaBloco(img->y, 8*((i*8)/img->w), (i*8)%img->w, DCT_inversa);
        //printf("Y[%d][%d]\n", 8*((i*8)/img->w), (i*8)%img->w);

        desalocarBloco(&bloco);
        desalocarBloco(&quantizacao_inversa);
        desalocarBloco(&DCT_inversa);
    }

    // Componente Cb.
    for(int i = 0; i < num_blocos_CbCr; i++){
        bloco = monta_bloco(blocos_em_vetor_Cb[i], 'B');
        quantizacao_inversa = desfazQuantizacao(bloco);
        DCT_inversa = desfazDCT(quantizacao_inversa);
        gravaBloco(img->cb, 8*((i*8)/img->cbcr_w), (i*8)%img->cbcr_w, DCT_inversa);
        //printf("Cb[%d][%d]\n", 8*((i*8)/img->cbcr_w), (i*8)%img->cbcr_w);

        desalocarBloco(&bloco);
        desalocarBloco(&quantizacao_inversa);
        desalocarBloco(&DCT_inversa);
    }

    // Componente Cr.
    for(int i = 0; i < num_blocos_CbCr; i++){
        bloco = monta_bloco(blocos_em_vetor_Cr[i], 'R');
        quantizacao_inversa = desfazQuantizacao(bloco);
        DCT_inversa = desfazDCT(quantizacao_inversa);
        gravaBloco(img->cr, 8*((i*8)/img->cbcr_w), (i*8)%img->cbcr_w, DCT_inversa);
        //printf("Cr[%d][%d]\n", 8*((i*8)/img->cbcr_w), (i*8)%img->cbcr_w);

        desalocarBloco(&bloco);
        desalocarBloco(&quantizacao_inversa);
        desalocarBloco(&DCT_inversa);
    }

    // Transformação para RGB.
    for(int i = 0; i < img->h; i++) {
        for(int j = 0; j < img->w; j++) {
            double cr = (img->cr)[i/2][j/2] - 128.0;
            double cb = (img->cb)[i/2][j/2] - 128.0;

            double r_ = (img->y)[i][j] + 1.402*cr;
            double g_ = (img->y)[i][j] - 0.344*cb - 0.714*cr;
            double b_ = (img->y)[i][j] + 1.772*cb;

            (img->r)[i][j] = (unsigned char) max(0, min(255, r_));
            (img->g)[i][j] = (unsigned char) max(0, min(255, g_));
            (img->b)[i][j] = (unsigned char) max(0, min(255, b_));
        }
    }

    printf("chegou aqui\n");

    encerrarDecodificacao(); // Desaloca as árvores.
    destruirLeitor(&leitor);

    printf("chegou aqui 2\n");

    desalocarMatriz_int(&blocos_em_vetor_Y, num_blocos_Y);
    desalocarMatriz_int(&blocos_em_vetor_Cb, num_blocos_CbCr);
    desalocarMatriz_int(&blocos_em_vetor_Cr, num_blocos_CbCr);

    return img;
}