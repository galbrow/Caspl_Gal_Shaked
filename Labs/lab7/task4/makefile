all: exec

exec: hexeditplus.c
	gcc -g -m32 -c -o hexeditplus.o hexeditplus.c
	gcc -g -m32  hexeditplus.o -o hexeditplus
	rm hexeditplus.o

.PHONY: clean
clean:
	rm -rf ./*.o hexeditplus hexeditplus.o
