all: compressor descompressor

compressor: consts util codifica gravador bloco header imagem decodifica leitor arvore
	gcc -std=c99 compressor.c -o compressor ./utils/header.o ./utils/codifica.o ./utils/imagem.o ./utils/util.o ./utils/consts.o ./utils/bloco.o ./utils/gravador.o ./utils/decodifica.o ./utils/leitor.o ./utils/arvore.o

descompressor: consts util codifica gravador bloco header imagem decodifica leitor arvore
	gcc -std=c99 descompressor.c -o descompressor ./utils/header.o ./utils/codifica.o ./utils/imagem.o ./utils/util.o ./utils/consts.o ./utils/bloco.o ./utils/gravador.o ./utils/decodifica.o ./utils/leitor.o ./utils/arvore.o

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

codifica:
	gcc -c ./utils/codifica.c -o ./utils/codifica.o

decodifica:
	gcc -c ./utils/decodifica.c -o ./utils/decodifica.o

gravador:
	gcc -c ./utils/gravador.c -o ./utils/gravador.o

leitor:
	gcc -c ./utils/leitor.c -o ./utils/leitor.o
arvore:
	gcc -c ./utils/arvore.c -o ./utils/arvore.o

clean:
	rm compressor descompressor ./utils/*.o ./imgComprimida/* ./imgDescomprimida/*