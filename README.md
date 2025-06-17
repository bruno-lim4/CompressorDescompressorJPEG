# CompressorDescompressorJPEG

Este projeto implementa um compressor e descompressor de imagens no formato BMP, utilizando técnicas baseadas no padrão JPEG, como a Transformada Discreta de Cosseno (DCT), quantização e codificação Huffman.

## Índice

- [Estrutura do Projeto](#estrutura-do-projeto)
- [Funcionalidades](#funcionalidades)
- [Requisitos](#requisitos)
- [Como Compilar](#como-compilar)
- [Como Usar](#como-usar)

## Estrutura do Projeto

A estrutura do projeto é organizada da seguinte forma:

- **Compressor**: Localizado em `src/compressor/`, possui funções e estruturas utilizadas exclusivamente pelo compressor.
- **Descompressor**: Localizado em `src/descompressor/`, possui funções e estruturas utilizadas exclusivamente pelo descompressor.
- **Biblioteca Comum**: Localizada em `src/common/`, contém funções e estruturas compartilhadas entre o compressor e o descompressor, como manipulação de blocos, cabeçalhos BMP e operações em cadeias de bits, estrutura da Imagem (e suas matrizes de cor)...

## Funcionalidades

### Compressão
- Conversão de imagens BMP para o espaço de cores YCbCr, com downsampling 4:2:0.
- Aplicação da Transformada Discreta de Cosseno (DCT) em blocos 8x8.
- Quantização dos coeficientes da DCT.
- Aplicação de um conjunto de codificações entrópicas para a compressão.

### Descompressão
- Decodificação de codificações entrópicas aplicadas na compressão.
- Desquantização e aplicação da DCT inversa.
- Reconstrução da imagem no espaço de cores RGB.

## Requisitos

- **Compilador**: GCC (ou outro compilador C compatível).
- **Sistema Operacional**: Linux ou outro sistema compatível com `Makefile`.

## Como Compilar

Use o `Makefile` para compilar o projeto. Os binários gerados serão armazenados no diretório `bin/`.

1. Para compilar o compressor e o descompressor:
   ```bash
   make
   ```

2. Para limpar os arquivos gerados durante a compilação:
   ```bash
   make clean
   ```

## Como Usar

Após compilar o projeto, você pode executar o compressor e o descompressor utilizando os binários gerados no diretório `bin/`.

### Compressor
Para comprimir uma imagem BMP, execute:
```bash
./bin/compressor -i <caminho_da_imagem_bmp> -o <caminho_saida_arquivo_bin_compactado>
```

Exemplo:
```bash
./bin/compressor -i imagens/exemplo.bmp -o saida/exemplo.bin
```

### Descompressor
Para descomprimir um arquivo compactado, execute:
```bash
./bin/decompressor -i <caminho_arquivo_bin_compactado> -o <caminho_saida_imagem_bmp>
```

Exemplo:
```bash
./bin/decompressor -i saida/exemplo.bin -o imagens/exemplo_reconstruido.bmp
```

Certifique-se de que os caminhos fornecidos existam e tenham permissões de leitura e escrita.

