section .text:
%macro checkArgs 1
    cmp byte [index], %1                ;for 2 arguments sends 2
    jnb %%endCheckArgs
    push errorLackOfArgs
    push [stderr]
    call fprintf
    jmp  startLoop                       ;else jump to start of loop ask if sub changes cf 
    %%endCheckArgs:
%endmacro

section .data:
    errorOutOfBound db 'Error: Operand Stack Overflow',0xA
    errorOutOfBoundLen equ $-errorOutOfBound
    errorLackOfArgs db 'Error: Insufficient Number of Arguments on Stack',0xA
    errorLackOfArgsLen equ $-errorLackOfArgs
    counter: dd 0
    numOfOperations: dd 0
global myCalc
extern malloc
extern fgets
extern printf
extern stdin
extern stdout
extern stderr
extern fprintf

section .rodata
        CALC:   DB "calc:" , 10 , 0         ;format string 

section .bss:
    buffer: resb 80                         ;ask peter for resd
    stack: resd 1
    num: resd 1

;stack = ecx
myCalc:
    push ebp
	mov ebp, esp	
	pushad	
    mov ecx, dword [ebp+8]              ;ecx= arg[1]
    ;init stack
    shl ecx,2
    push ecx
    call malloc
    mov [stack], eax
    ;end init stack

    .loop:
    ;print calc:
    push CALC
    call printf             ;x
    add esp, 4                          ;print calc:
    ;end print calc:

    ;get input from user
    push dword [stdin]
    push dword 80
    push dword buffer
    call fgets
    add esp, 12                         ;buffer = fgets
    ;end get input from user

    ;start q case
    cmp byte [buffer],113
    jnz myCalc.endQCase
    mov eax,1
    int 0x80                            ;system call exit
    .endQCase
    ;end q case


    ;start + case  
    cmp byte [buffer],43
    checkArgs 2                         ;check if there is enough arguments
    mov ebx, [stack]                    ;arg0
    sub dword stack,1                   ;index--
    mov ecx, [stack]                    ;arg1
    sub dword stack,1                   ;index--
    ;------------------------------------------------------------------do add with the object linked list
    add ebx, ecx                        ;ebx=ebx+ecx
    ;------------------------------------------------------------------
    mov [stack], ebx                    ;stack[index]=ebx
    add dword stack, 1                  ;index++
    jmp myCalc.loop
    ;end + case

    ;compare buffer to "p" 
    cmp byte [buffer],112 
    checkArgs 1                         ;check if there is enough arguments
    mov ebx, [stack]                    ;arg0
    sub dword stack,1                   ;index--
    ;----------------------------------------
    push ebx
    ;---------------convert to string/integer
    push [stdout]
    call printf
    add esp, 8
    mov stack[index], ebx               ;stack[index]=ebx
    inc index                           ;index++

    ;


    ;enter number
    mov edx, 0                          ;the accumulator value
    push 5
    call malloc
    add esp, 4
    mov dword [stack], eax              ;stack[i]=malloc
    mov ebx,eax                         ;linkedL* ebx=eax
    mov ecx,buffer
    .getLastIndex                       ;check if end of string
    cmp byte [ecx], 0                
    jz myCalc.endIndex                  
    inc ecx
    jmp myCalc.getLastIndex
    .endIndex                           ;end check ecx=buffer length
    dec ecx
    ;end with shaked
    .startMallocLoop
    ;37211257

    ;num = 7
    ;num = 5--->shr 4
    ;num = 2--->shr 8

    mov eax,[ecx]                       ;eax = '7'
    sub eax,48                          ;eax = 7   111
    mov dword [num],eax                 ;num= 7    111
    dec ecx                             ;ecx --
    ;case that there is q after the first read
    cmp byte [buffer],113               
    jnz notQafterFirstLetter            ;jump to end of label if there is no q
    shr [num],word [counter]            ;ask peter about word/dword
    add dword [num], edx                ;ask peter about word/dword
    mov edx, 0                          ;reset edx
    mov byte [ebx],byte [num]           ;node.value = 10101111 --> AF
    jmp myCalc.loop                     ;finish iteration of getting number
    .notQafterFirstLetter
    ;end case
    mov eax,[ecx]                       ;eax = '5'
    sub eax,48                          ;eax = 5   101
    shr eax , 3                         ;eax = 40  101000
    add dword [num], eax                ;num = 57  101111
    dec ecx                             ;ecx --
    ;
    jnz notQafterSecondLetter
    shr [num],word [counter]            ;ask peter about word/dword
    add dword [num], edx                ;ask peter about word/dword
    mov edx, 0                          ;reset edx
    mov byte [ebx],byte [num]           ;node.value = 10101111 --> AF
    jmp myCalc.loop                     ;finish iteration of getting number
    .notQafterSecondLetter
    ;
    cmp word [counter], 2               ;if (counter == 2)
    jnz myCalc.caseNotTwoBytes          ;case that counter isnt 2 jump to resume normal
    mov word [counter], 0               ;reset counter
    shl [num],2                         ;shift num 2 bytes for adding
    add dword [num],edx                 ;add accumulator to num
    mov edx, 0                          ;reset edx
    mov byte [ebx],byte [num]           ;node.value = 10101111 --> AF
    jmp myCalc.newNode                  ;finish iteration and start getting new value from user
    .caseNotTwoBytes                    ;end if
    mov eax,[ecx]                       ;eax = '2'
    sub eax,48                          ;eax = 2   010
    shr eax , 6                         ;eax = 200 010000000 
    add dword [num], eax                ;num = 257 010101111  ---> 0
    shr [num],word [counter]            ;ask peter about word/dword
    add dword [num], edx                ;ask peter about word/dword
    mov edx, 0                          ;reset edx
    mov byte [ebx],byte [num]           ;node.value = 10101111 --> AF
    shl [num],8                         ;shift num 8 bytes to get the leftmost bytes
    mov edx, word [num]                 ;edx(counter) = 0
    inc word [counter]                  ;counter ++
    dec ecx                             ;ecx --
    ;
    cmp byte [buffer],113               ;case that we finished read 3 letters and have q on the next char
    jnz notQafterThirdLetter            ;case that there is no q jump to resumer
    cmp edx,0                           ;check if there is left value in the accumulator edx 
    jnz .myCalc.endEDX0                 ;if there is value do not add proccess
    jmp myCalc.loop                     ;else end process and start new loop 
    .endEDX0                            
    push 5                              ;create new node
    call malloc
    add esp, 4                          ;fix esp
    mov byte [eax],edx                  ;mov to the node the acccumulator edx value
    mov dword [ebx + 1],eax             ;ebx.next=new node ask peter if thats the way to do it
    jmp myCalc.loop                     ;start new loop
    .notQafterThirdLetter
    ;
    .newNode
    ;create new node
    push 5                              ;create new node
    call malloc
    add esp, 4                          ;fix esp pointer
    ;end create new node in eax         
    ;********* ask peter ****
    mov dword [ebx + 1],eax             ;ebx.next=new node ask peter if thats the way to do it
    mov ebx, eax                        ;ebx = eax
    jmp myCalc.startMallocLoop
    .endLoop

    