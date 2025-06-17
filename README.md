# Compressor/Descompressor de Imagens (Estilo JPEG)

Uma implementação em C do pipeline de compressão e descompressão de imagens, aplicando conceitos fundamentais do padrão JPEG em arquivos de formato BMP.

## Índice

- [Sobre o Projeto](#sobre-o-projeto)
  - [Fluxo de Compressão](#fluxo-de-compressão)
  - [Fluxo de Descompressão](#fluxo-de-descompressão)
- [Principais Funcionalidades](#-principais-funcionalidades)
- [Estrutura do Projeto](#-estrutura-do-projeto)
- [Começando](#começando)
  - [Pré-requisitos](#pré-requisitos)
  - [Compilação](#compilação)
  - [Execução](#execução)

## Sobre o Projeto

Este projeto foi desenvolvido para explorar e aplicar as etapas clássicas de compressão de imagem com perdas. Ele lê uma imagem BMP não compactada, a processa através de um pipeline de compressão e gera um arquivo binário customizado (`.bin`). O processo inverso também é implementado, reconstruindo a imagem a partir do arquivo compactado.

### Fluxo de Compressão
```
BMP  ->  RGB para YCbCr  ->  Downsampling 4:2:0  ->  DCT  ->  Quantização  ->  Codificação (RLE + Huffman)  ->  Arquivo .bin
```
### Fluxo de Descompressão
```
Arquivo .bin  ->  Decodificação (RLE + Huffman)  ->  Dequantização  ->  DCT Inversa  ->  Upsampling  ->  YCbCr para RGB  ->  BMP
```

## 🌟 Principais Funcionalidades

* **Conversão de Cores**: Converte imagens do espaço de cores **RGB** para **YCbCr**, separando luminância (Y) de crominância (Cb, Cr).
* **Subamostragem (Downsampling)**: Aplica o downsampling de crominância **4:2:0** para reduzir a quantidade de dados de cor, explorando a menor sensibilidade do olho humano a variações de cor.
* **Transformada Discreta de Cosseno (DCT)**: Processa a imagem em blocos 8x8, transformando dados espaciais em dados de frequência, o que concentra a maior parte da energia nos coeficientes de baixa frequência.
* **Quantização**: Reduz a precisão dos coeficientes da DCT, descartando informações menos perceptíveis. Esta é a principal etapa de compressão com perdas.
* **Codificação Entrópica**:
    * **RLE (Run-Length Encoding)** para os coeficientes AC, compactando sequências de zeros.
    * **Codificação Huffman** para os coeficientes DC e os valores resultantes do RLE, atribuindo códigos menores aos valores mais frequentes. Usamos uma tabela pronta.

## 📂 Estrutura do Projeto

O código-fonte está organizado de forma modular para separar as responsabilidades do compressor, do descompressor e do código comum.

```
.
├── src/
│   ├── compressor/         # Lógica exclusiva da compressão
│   ├── descompressor/      # Lógica exclusiva da descompressão
│   └── common/             # Funções e estruturas compartilhadas
└── Makefile
```

## Começando

Siga as instruções abaixo para compilar e executar o projeto em um ambiente Linux.

### Pré-requisitos

* **GCC** (GNU Compiler Collection)
* **Make**

### Compilação

Use o `Makefile` para compilar o projeto. Os binários serão gerados em um novo diretório `bin/`.

```bash
# Compila o compressor e o descompressor
make

# Para limpar os arquivos gerados (binários e objetos)
make clean
```

### Execução

Após a compilação, use os executáveis `compressor` e `decompressor` conforme os exemplos abaixo.

#### Para Comprimir
```bash
./bin/compressor -i <caminho_imagem_entrada.bmp> -o <caminho_arquivo_saida.bin>
```

**Exemplo:**
```bash
./bin/compressor -i imagens/exemplo.bmp -o saida/exemplo.bin
```

#### Para Descomprimir
```bash
./bin/decompressor -i <caminho_arquivo_entrada.bin> -o <caminho_imagem_saida.bmp>
```
**Exemplo:**
```bash
./bin/decompressor -i saida/exemplo.bin -o imagens/exemplo_reconstruido.bmp
```
> **Nota**: Certifique-se de que os diretórios de entrada e saída existam e que você tenha permissões de leitura/escrita.