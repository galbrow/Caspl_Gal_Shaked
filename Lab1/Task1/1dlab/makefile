all: exec

exec: encoder
	gcc -g -m32 -c -o encoder.o encoder.c
	gcc -g -m32 encoder.o -o encoder

.PHONY: clean

clean:
	rm -rf ./*.o encoder
