all: exec

exec: digitCounter.c
	gcc -g -m32 -fno-pie -c -o digitCounter.o digitCounter.c
	gcc -g -m32 -fno-pie  digitCounter.o -o digitCounter
	rm digitCounter.o

.PHONY: clean
clean:
	rm -rf ./*.o digitCounter digitCounter.o
