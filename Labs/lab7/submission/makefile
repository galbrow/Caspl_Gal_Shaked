all: exec

exec: task1.c task4.c
	gcc -g -m32 -c -o task1.o task1.c
	gcc -g -m32  task1.o -o task1
	gcc -g -m32 -c -o task4.o task4.c
	gcc -g -m32  task4.o -o task4
	rm task1.o
	rm task4.o

.PHONY: clean
clean:
	rm -rf ./*.o task1 task1.o
	rm -rf ./*.o task4 task4.o
