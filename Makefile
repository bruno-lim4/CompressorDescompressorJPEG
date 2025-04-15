all: main

main: util header imagem
	gcc -std=c99 compressor.c -o compressor ./utils/header.o ./utils/imagem.o ./utils/util.o

header:
	gcc -c ./utils/header.c -o ./utils/header.o

imagem:
	gcc -c ./utils/imagem.c -o ./utils/imagem.o

util:
	gcc -c ./utils/util.c -o ./utils/util.o

clean:
	rm compressor ./utils/*.o