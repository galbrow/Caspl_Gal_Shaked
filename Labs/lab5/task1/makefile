all: exec

libs: c-lib

c-lib: LineParser.c
	gcc -g -m32 -c -o LineParser.o LineParser.c

exec: myshell.c libs
	gcc -g -m32 -c -o myshell.o myshell.c
	gcc -g -m32  myshell.o LineParser.o -o myshell
	rm myshell.o LineParser.o

.PHONY: clean
clean:
	rm -rf ./*.o myshell LineParser.o myshell.o
