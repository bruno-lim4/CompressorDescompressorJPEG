#include "./common/imagem_comprimir.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  

void print_help() {
    printf("Uso: compressor -i <input.bmp> -o <output.bin>\n");
    printf("Opções:\n");
    printf("  -i    Caminho da imagem BMP de entrada\n");
    printf("  -o    Caminho do arquivo .bin de saída\n");
    printf("  -h    Mostrar esta ajuda\n");
}

int main(int argc, char *argv[]) {
    char *input_path = NULL;
    char *output_path = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "i:o:h")) != -1) {
        switch (opt) {
            case 'i': input_path = optarg; break;
            case 'o': output_path = optarg; break;
            case 'h': print_help(); return 0;
            default:
                fprintf(stderr, "Argumento inválido.\n");
                print_help();
                return 1;
        }
    }

    if (!input_path || !output_path) {
        fprintf(stderr, "Erro: é necessário informar entrada (-i) e saída (-o).\n");
        print_help();
        return 1;
    }

    FILE* img_file = fopen(input_path, "rb");
    if (!img_file) {
        perror("Erro ao abrir imagem de entrada");
        return 1;
    }

    FILE* bin_file = fopen(output_path, "wb");
    if (!bin_file) {
        perror("Erro ao abrir arquivo de saída");
        fclose(img_file);
        return 1;
    }

    IMAGEM* img = criarImagem(img_file);
    comprimeImagem(img, bin_file);

    desalocarImagem(&img);
    fclose(img_file);
    fclose(bin_file);

    printf("Compressão finalizada com sucesso!\n");
    return 0;
}
