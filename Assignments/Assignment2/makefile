all: exec

exec: calc.s
	nasm -g -f elf -o calc.o calc.s
	rm calc.o

.PHONY: clean
clean:
	rm -rf ./*.o calcs