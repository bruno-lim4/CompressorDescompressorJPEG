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
    if (img == NULL) return NULL;

    // pega o cabecalho
    img->fileHeader = leituraFileHeader(f);
    img->infoHeader = leituraInfoHeader(f);

    printFileHeader(img->fileHeader);
    printInfoHeader(img->infoHeader);

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
    img->cbcr_w = (img->w)/2 + ( (((img->w)/2)%8) ? (8 - ((img->w)/2)%8) : 0);
    img->cbcr_h = (img->h)/2 + ( (((img->h)/2)%8) ? (8 - ((img->h)/2)%8) : 0);

    printf("cbcrNovo = (%d, %d)\n", img->cbcr_h, img->cbcr_w);

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

    // antes de retornar, coloca todos os valores no intervalo [0, 255]
    for(int i = 0; i < img->h; i++) {
        for(int j = 0; j < img->w; j++) {
            if (i < img->cbcr_w && j < img->cbcr_w) {
                img->cb[i][j] =  max(0, min(255, img->cb[i][j]));
                img->cr[i][j] =  max(0, min(255, img->cr[i][j]));
            }
            img->y[i][j] =  max(0, min(255, img->y[i][j]));
        }
    }

    return img;
}

void comprime_gravaMatriz(GRAVADOR* gravador, double** m, int i, int j, char tipoMatriz);

void comprimeImagem(IMAGEM* img, FILE* f) {
    escreverFileHeader(f, img->fileHeader);
    escreverInfoHeader(f, img->infoHeader);

    GRAVADOR* gravador = criarGravador(f);

    comprime_gravaMatriz(gravador, img->y, img->h, img->w, 'L');
    comprime_gravaMatriz(gravador, img->cb, img->cbcr_h, img->cbcr_w, 'B');
    comprime_gravaMatriz(gravador, img->cr, img->cbcr_h, img->cbcr_w, 'R');

    finalizarGravacao(gravador);
    desalocarGravador(&gravador);
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

void codifica_gravaVetor(GRAVADOR* gravador, int* vetor, int valor_difDC) {
    printf("\nvetor de 64 pos (dcDif: %d): ", valor_difDC);
    for(int i = 0; i < 64; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");

    // grava DC
    int qtd_DC;
    printf("codifica DC=%d: ", valor_difDC);
    uint32_t codificaDC = codifica_infoDC(valor_difDC, &qtd_DC);
    print_binary(codificaDC, qtd_DC);
    gravarValor(gravador, codificaDC, qtd_DC);

    int qtd_zeros = 0;

    for(int i = 1; i < 64; i++) {
        if (vetor[i] == 0) {
            qtd_zeros++;
        } else {
            int valor = vetor[i];

            while(qtd_zeros >= 15) {
                int qtd_15;
                printf("codifica AC=(zero=%d, value=%d): ", qtd_zeros, 0);
                uint32_t pref = codifica_infoAC(15, 0, &qtd_15);
                print_binary(pref, qtd_15);
                gravarValor(gravador, pref, qtd_15);

                qtd_zeros -= 15;
            }

            int qtd_AC;
            printf("codifica AC=(zero=%d, value=%d): ", qtd_zeros, valor);
            uint32_t codificaAC = codifica_infoAC(qtd_zeros, valor, &qtd_AC);
            print_binary(codificaAC, qtd_AC);
            gravarValor(gravador, codificaAC, qtd_AC);

            qtd_zeros = 0;
        }
    }

    int qtd_fim;
    uint32_t codifica_fim;

    codifica_fim = codifica_infoAC(0, 0, &qtd_fim);
    printf("codifica AC - FIM DO BLOCO: ");
    print_binary(codifica_fim, qtd_fim);
    gravarValor(gravador, codifica_fim, qtd_fim);
}

void comprime_gravaMatriz(GRAVADOR* gravador, double** m, int h, int w, char tipoMatriz) {
    int ultimo_dc = 0;

    for(int i = 0; i < h; i += 8) {
        for(int j = 0; j < w; j += 8) {
            BLOCO* bloco = criarBloco(m, i, j, tipoMatriz);
            int* vetor_final = processaBloco(bloco);
            codifica_gravaVetor(gravador, vetor_final, vetor_final[0]-ultimo_dc);
            ultimo_dc = vetor_final[0];

            free(vetor_final); vetor_final = NULL;
            desalocarBloco(&bloco);
        }
    }
}



IMAGEM* descomprimeImagem(FILE* in, FILE* out){
    iniciarDecodificacao(); // Inicializa árvores de prefixos.
    IMAGEM* img = malloc(sizeof(IMAGEM));

    img->fileHeader = leituraFileHeader(in);
    img->infoHeader = leituraInfoHeader(in);

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
    }

    ehPrimeiroDC = 1;
    DC_anterior = 0;
    for(int i = 0; i < num_blocos_CbCr; i++){
        decodificaDC(&(blocos_em_vetor_Cb[i][0]), leitor, DC_anterior, &ehPrimeiroDC);
        DC_anterior = blocos_em_vetor_Cb[i][0];

        decodificaAC(blocos_em_vetor_Cb[i], leitor);
    }

    ehPrimeiroDC = 1;
    DC_anterior = 0;
    for(int i = 0; i < num_blocos_CbCr; i++){
        decodificaDC(&(blocos_em_vetor_Cr[i][0]), leitor, DC_anterior, &ehPrimeiroDC);
        DC_anterior = blocos_em_vetor_Cr[i][0];

        decodificaAC(blocos_em_vetor_Cr[i], leitor);
    }

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
        printf("[%d][%d]\n", 8*((i*8)/img->cbcr_w), (i*8)%img->cbcr_w);

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

    encerrarDecodificacao(); // Desaloca as árvores.
    destruirLeitor(&leitor);

    desalocarMatriz_int(&blocos_em_vetor_Y, num_blocos_Y, 64);
    desalocarMatriz_int(&blocos_em_vetor_Cb, num_blocos_CbCr, 64);
    desalocarMatriz_int(&blocos_em_vetor_Cr, num_blocos_CbCr, 64);

    return img;
}

// Salva a imagem no arquivo.
void salvarImagem(FILE* f, IMAGEM* img) {
    // Escreve cabeçalhos
    fseek(f, 0, SEEK_SET);
    escreverFileHeader(f, img->fileHeader);
    escreverInfoHeader(f, img->infoHeader);

    int largura = img->w;
    int altura = img->h;

    for (int i = 0; i < altura; i++){
        for (int j = 0; j < largura; j++) {
            fwrite(&img->b[i][j], 1, 1, f); // Azul
            fwrite(&img->g[i][j], 1, 1, f); // Verde
            fwrite(&img->r[i][j], 1, 1, f); // Vermelho
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