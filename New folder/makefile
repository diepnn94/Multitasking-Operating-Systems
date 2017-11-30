all: nsh

nsh.o: nsh.c 
	gcc -ggdb -c nsh.c

parse.o: parse.c
	gcc -ggdb -c parse.c

nsh: nsh.o parse.o
	gcc -ggdb nsh.o parse.o -o nsh

clean:
	rm -rf nsh nsh.o parse.o