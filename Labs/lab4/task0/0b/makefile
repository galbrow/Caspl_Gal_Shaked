all: exec

libs: asm-lib

asm-lib: lab4_start.s
	# flags meanings:
	# -g = debug symbolx
	# -f elf = output format (ELF 32 bit)
	nasm -f elf32 lab4_start.s -o lab4_start.o


exec: lab4_main.c libs
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector lab4_util.c -o lab4_util.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector lab4_main.c -o lab4_main.o
	ld -m elf_i386 lab4_start.o lab4_main.o lab4_util.o -o lab4_main

.PHONY: clean
clean:
	rm -rf ./*.o lab4_main
