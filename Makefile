all: main

main: consts util gravar bloco header imagem
	gcc -std=c99 compressor.c -o compressor ./utils/header.o ./utils/gravar.o ./utils/imagem.o ./utils/util.o ./utils/consts.o ./utils/bloco.o

imagem:
	gcc -c ./utils/imagem.c -o ./utils/imagem.o

header:
	gcc -c ./utils/header.c -o ./utils/header.o

bloco:
	gcc -c ./utils/bloco.c -o ./utils/bloco.o

util:
	gcc -c ./utils/util.c -o ./utils/util.o

consts:
	gcc -c ./utils/consts.c -o ./utils/consts.o

gravar:
	gcc -c ./utils/gravar.c -o ./utils/gravar.o

clean:
	rm compressor ./utils/*.o