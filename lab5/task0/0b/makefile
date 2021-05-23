all: exec

exec: looper.c
	gcc -g -m32 -c -o looper.o looper.c
	gcc -g -m32 looper.o -o looper
	rm looper.o 

.PHONY: clean
clean:
	rm -rf ./*.o looper looper.o
