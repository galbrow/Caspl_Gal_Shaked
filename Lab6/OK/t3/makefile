all: exec

libs: c-lib

c-lib: LineParser.c
	gcc -g -m32 -c -o LineParser.o LineParser.c

exec: task3.c libs
	gcc -g -m32 -c -o task3.o task3.c
	gcc -g -m32  task3.o LineParser.o -o task3
	rm task3.o LineParser.o

.PHONY: clean
clean:
	rm -rf ./*.o task3 LineParser.o task3.o
