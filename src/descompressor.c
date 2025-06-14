#include "./common/imagem_descomprimir.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // getopt()

void print_help() {
    printf("Uso: descompressor -i <input.bin> -o <output.bmp>\n");
    printf("Opções:\n");
    printf("  -i    Caminho do arquivo .bin de entrada\n");
    printf("  -o    Caminho do arquivo .bmp de saída\n");
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
        fprintf(stderr, "Erro: entrada (-i) e saída (-o) são obrigatórias.\n");
        print_help();
        return 1;
    }

    FILE* in = fopen(input_path, "rb");
    if (!in) {
        perror("Erro ao abrir arquivo de entrada");
        return 1;
    }

    FILE* out = fopen(output_path, "wb");
    if (!out) {
        perror("Erro ao abrir arquivo de saída");
        fclose(in);
        return 1;
    }

    IMAGEM* img = descomprimeImagem(in);
    salvarImagem(out, img);

    desalocarImagem(&img);
    fclose(in);
    fclose(out);

    printf("Descompressão finalizada com sucesso!\n");
    return 0;
}
