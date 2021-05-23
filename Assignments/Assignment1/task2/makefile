all: exec

libs: asm-lib

asm-lib: asm_task2Assignment1.s
	# flags meanings:
	# -g = debug symbolx
	# -f elf = output format (ELF 32 bit)
	# -o add.o = write assembled file to add.o
	nasm -g -f elf -o asm_task2Assignment1.o asm_task2Assignment1.s


exec: main_task2Assignment1.c libs
	gcc -g -m32 -c -o main_task2Assignment1.o main_task2Assignment1.c
	gcc -g -m32  main_task2Assignment1.o asm_task2Assignment1.o  -o main_task2Assignment1
	rm main_task2Assignment1.o asm_task2Assignment1.o

.PHONY: clean
clean:
	rm -rf ./*.o main_task2Assignment1
