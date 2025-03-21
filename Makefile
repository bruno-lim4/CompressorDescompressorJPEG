all: main

main: util
	gcc -std=c99 main.c -o main util.o

util:
	gcc -c util.c -o util.o

clean:
	rm main *.o