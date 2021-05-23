all: exec

exec: mypipe.c
	gcc -g -m32 -c -o mypipe.o mypipe.c
	gcc -g -m32  mypipe.o -o mypipe
	rm mypipe.o

.PHONY: clean
clean:
	rm -rf ./*.o mypipe mypipe.o
