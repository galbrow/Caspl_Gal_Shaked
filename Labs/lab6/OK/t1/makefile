all: exec

exec: task1.c
	gcc -g -m32 -c -o task1.o task1.c
	gcc -g -m32  task1.o -o task1
	rm task1.o

.PHONY: clean
clean:
	rm -rf ./*.o task1 task1.o
