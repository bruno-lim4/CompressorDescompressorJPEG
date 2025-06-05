#include "imagem.h"
#include "bloco.h"
#include "gravador.h"
#include "codifica.h"

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
    printf("====IMPRESSÃO DA IMAGEM====\n%d % d\n", img->cbcr_h, img->cbcr_w);

    for(int i = 0; i < img->h; i++){
        for(int j = 0; j < img->w; j++){
            printf("%4d %.4d %4d\n", (img->r)[i][j], (img->g)[i][j], (img->b)[i][j]);
        }
        printf("\n");
    }

    return;
}

//Esboço das funções a serem implementadas para descompressão da imagem.

// Esboço da função que faz a decodificação de Huffman, codificação por diferenças e RLE
// e retorna um vetor de blocos, que passarão pela decodificação da quantização.
void desfazHuffmanDifRLE(){ //MIGRAR PARA A FUNÇÃO descomprimeImagem().
    // Lê o cabeçalho e os dados adicionais e armazena as dimensões da imagem original em h e w.
    int h, w;

    // Decodifica primeiro os valores referentes a Y.
    int num_blocos_Y = (h/8)*(w/8);
    int* DC = (int*) malloc(sizeof(int)*num_blocos_Y);
    int** blocos_em_vetor_Y = (int**) malloc(sizeof(int*)*num_blocos_Y); // Já vai armazenar o bloco inteiro, includindo coef. DC.
    for(int i = 0; i < num_blocos_Y; i++){
        blocos_em_vetor_Y[i] = (int) malloc(sizeof(int)*64);
        blocos_em_vetor_Y[i][0] = DC[i];
    }

    



    int bloco_hExtra = ((h/2) % 8) ? 1 : 0;
    int bloco_wExtra = ((w/2) % 8) ? 1 : 0;
    int num_blocos_CbCr = (h/2 + bloco_hExtra)*(w/2 + bloco_wExtra);



    //Depois da decodificação completa do bitstream, temos um vetor de vetores,
    //cada um contendo os dados de um bloco.
    BLOCO* blocos = criarVetorBlocos(num_blocos_Y);
    for(int i = 0; i < num_blocos_Y; i++){
        blocos[i] = monta_bloco(blocos_em_vetor_Y[i], 'L');
    }

    //Nesse ponto, temos todos os blocos que compõem a matriz Y.
    //DECIDIR: mandar esse vetor de Y para outras funções, terminar sua decodificação, e 
    //seguir para Cb e Cr nessa mesma função.
    //OU retornar esse vetor, e fazer outra função que decodificará Cb e Cr do zero.
    //OUTRA IDEIA (provavelmente a melhor): a main deve simplesmente chamar uma função 'descomprimeImagem()', e essa
    //função simplesmente retorna a imagem pronta.

}

IMAGEM* descomprimeImagem(file *f){
    //FLUXO DA FUNÇÃO:
    //Lê cabeçalho e dados adicionais.
    /*...Assumindo que a ordem no arquivo seja DC(Y), AC(Y), DC(Cb), AC(Cb), DC(Cr), AC(Cr)...*/
    //Decodifica Huffman e obtém os DC de Y; guarda-os em um vetor.
    //Decodifica Huffman e obtém os AC de Y; guarda-os em um vetor de vetores junto com os DC.
    //Transforma cada vetor em um bloco; tem-se, agora, um vetor de blocos.
    //Passa o vetor a cada etapa seguinte.
    //Faz o mesmo para Cb e Cr.
    //Da forma que está no momento, passa os vetores de blocos para a função construirImagem(), que
    //transformará para RGB e guardará os dados numa struct IMAGEM.
    //Aí é só gravar os dados num arquivo .bmp :).

}

double* desfazCodDiferencial(double *cod, int tam){
    double* res = malloc(sizeof(double)*tam);

    res[0] = cod[0];
    for(int i = 1; i < tam; i++){
        res[i] = res[i-1] + cod[i];
    }

    return res;
}

double* desfazRLE(){

}

//Construção da struct IMAGEM resultante da descompressão.
//Recebe um vetor com todos os blocos que compõem a imagem.
//PRECISA DE VETORES PRA Y, CB E CR SEPARADAMENTE.
IMAGEM* construirImagem(double ***blocos, int img_h, int img_w){
    IMAGEM* img = (IMAGEM*) malloc(sizeof(IMAGEM));
    img->h = img_h;
    img->w = img_w;
    img->cbcr_h = img->h/2; //Ignora o preenchimento feito para ficar múltiplo de 8.
    img->cbcr_w = img->w/2;

    alocarMatriz_unsignedChar(&(img->r), img->h, img->w);
    alocarMatriz_unsignedChar(&(img->g), img->h, img->w);
    alocarMatriz_unsignedChar(&(img->b), img->h, img->w);

    alocarMatriz_double(&(img->y), img->h, img->w);
    alocarMatriz_double(&(img->cb), img->cbcr_w, img->cbcr_h);
    alocarMatriz_double(&(img->cr), img->cbcr_w, img->cbcr_h);

    // Preenchendo as matrizes Cb e Cr.
    for(int i = 0; i < img->cbcr_h; i++){ // Índice percorrendo as linhas da imagem.
        for(int j = 0; j < img->cbcr_w; j++){ // Índice percorrendo as colunas da imagem.
            int index = (img->cbcr_w/8) * (i/8) + (j/8); //Índice para percorrer o vetor de blocos.
            //printf("(img->cb)[%d][%d] = %5.0lf\n", i, j, (blocos[index])[i % 8][j % 8]);
            (img->cb)[i][j] = (blocos[index])[i % 8][j % 8];
            (img->cr)[i][j] = (blocos[index])[i % 8][j % 8];
        }
    }


/*
    // Preenchendo a matriz Y.
    for(int i = 0; i < img->h; i++){
        printf("i: %d\n", i);
        for(int j = 0; j < img->w; j++){
            int index = (img_w/8) * (i/8) + (j/8);
            (img->y)[i][j] = (blocos[index])[i % 8][j % 8];
        }
    }
*/

    // Transformação para RGB.
    int rgb_i = 0, rgb_j = 0;
    printf("==== TRANSFORMAÇÃO PARA RGB ====\n\nDimensões da imagem: %d x %d\ncbcr_h e _w: %d %d\n\n", img->h, img->w, img->cbcr_h, img->cbcr_w);
    for(int i = 0; i < img->cbcr_h; i++){
        rgb_j = 0;
        for(int j = 0; j < img->cbcr_w; j++){
            printf("i: %d j: %d\n", i, j);
            (img->r)[rgb_i][rgb_j] = ((img->cr)[i][j] - 128)/0.713 + (img->y)[rgb_i][rgb_j];
            (img->r)[rgb_i][rgb_j+1] = ((img->cr)[i][j] - 128)/0.713 + (img->y)[rgb_i][rgb_j+1];
            (img->r)[rgb_i+1][rgb_j] = ((img->cr)[i][j] - 128)/0.713 + (img->y)[rgb_i+1][rgb_j];
            (img->r)[rgb_i+1][rgb_j+1] = ((img->cr)[i][j] - 128)/0.713 + (img->y)[rgb_i+1][rgb_j+1];

            (img->b)[rgb_i][rgb_j] = ((img->cb)[i][j] - 128)/0.564 + (img->y)[rgb_i][rgb_j];
            (img->b)[rgb_i][rgb_j+1] = ((img->cb)[i][j] - 128)/0.564 + (img->y)[rgb_i][rgb_j+1];
            (img->b)[rgb_i+1][rgb_j] = ((img->cb)[i][j] - 128)/0.564 + (img->y)[rgb_i+1][rgb_j];
            (img->b)[rgb_i+1][rgb_j+1] = ((img->cb)[i][j] - 128)/0.564 + (img->y)[rgb_i+1][rgb_j+1];

            (img->g)[rgb_i][rgb_j] = (img->y)[rgb_i][rgb_j] - 0.344*((img->cb)[i][j]) - 0.714*(img->cr)[i][j];
            (img->g)[rgb_i][rgb_j+1] = (img->y)[rgb_i][rgb_j+1] - 0.344*((img->cb)[i][j]) - 0.714*(img->cr)[i][j];
            (img->g)[rgb_i+1][rgb_j] = (img->y)[rgb_i+1][rgb_j] - 0.344*((img->cb)[i][j]) - 0.714*(img->cr)[i][j];
            (img->g)[rgb_i+1][rgb_j+1] = (img->y)[rgb_i+1][rgb_j+1] - 0.344*((img->cb)[i][j]) - 0.714*(img->cr)[i][j];

            rgb_j += 2;
        }
        rgb_i += 2;
    }

    printf("Saiu da função construirImagem()\n");
    return img;
}

int get_img_w(IMAGEM *img){
    return img->w;
}