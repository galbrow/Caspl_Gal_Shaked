all: exec

exec: myELF.c
	gcc -g -m32 -c -o myELF.o myELF.c
	gcc -g -m32  myELF.o -o myELF
	rm myELF.o

.PHONY: clean
clean:
	rm -rf ./*.o myELF myELF.o
