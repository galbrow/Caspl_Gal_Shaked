;macro that changes eax value and new_node
%macro AddNewNode 0
	push 5
	call malloc
	add esp, 4 
%endmacro

%macro printDebug 1
    push %1
    push debug
    call printf
    add esp,4
%endmacro

%macro checkArgs 1
    cmp byte [index], %1                ;for 2 arguments sends 2
    jnb %%endCheckArgs
    push errorLackOfArgs
    push [stderr]
    call fprintf
    jmp  startLoop                       ;else jump to start of loop ask if sub changes cf 
    %%endCheckArgs:
%endmacro

;macro that changes ebx,eax registers and new_node Value
%macro AddNewNodeAndConnectToPrev 0
	pushad
	push dword 5
	call malloc
	add esp, 4 
	mov [eax], byte 0
	mov [eax+1], dword 0
    mov ebx,[new_node]
    mov [ebx+1], dword[eax]
	mov [new_node], eax
	popad
%endmacro

%macro setEbxToLastIndexBuffer 0
    mov ebx,buffer
    .getLastIndex:                       ;check if end of string
    cmp byte [ebx], 0                
    jz myCalc.endIndex                  
    inc ebx
    jmp myCalc.getLastIndex
    .endIndex:                           ;end check ecx=buffer length
    dec ebx
%endmacro

section .data:
    errorOutOfBound db 'Error: Operand Stack Overflow',0xA
    errorOutOfBoundLen equ $-errorOutOfBound
    errorLackOfArgs db 'Error: Insufficient Number of Arguments on Stack',0xA
    errorLackOfArgsLen equ $-errorLackOfArgs
    numOfOperations: dd 0
    maxIndex: dd 0
    index: dd 0
    counter: dd 0
    acccumulator: dd 0
    num: dd 0

section .rodata
        CALC:   DB "calc:" , 10 , 0         ;format string 
        debug: DB "here %i" ,10,0

section .bss
    buffer: resb 80                         ;ask peter for resd
    stack: resd 1
    StackFirst: resd 1
    currentNode: resd 1
    new_node: 	resd 1

section .text
  align 16
  global main
  extern printf
  extern fprintf 
  extern malloc 
  extern free 
  extern fgets 
  extern stdout
  extern stdin
  extern stderr
main:
    push ebp
    mov ebp, esp	
    pushad	
    mov ecx, dword [ebp + 12]              ;ecx= arg[1]
    mov ecx, dword [ecx]
    printDebug ecx
    mov ecx, dword [ecx]
    sub ecx, 48                         
    mov dword[maxIndex] ,ecx             ;add maxIndexValue
    myCalc:
        ;init stack
        printDebug 94
        shl ecx,2
        push ecx
        call malloc
        add esp, 4
        mov [StackFirst], eax
	    mov [stack], dword StackFirst
        ;end init stack

        .loop:
        ;print calc:
        push CALC
        call printf             ;x
        add esp, 4                          ;print calc:
        ;end print calc:

        ;get input from user
        ; push dword [stdin]
        ; push dword 80
        ; push dword buffer
        ; call fgets
        ; add esp, 12                         ;buffer = fgets
        ;end get input from user

        ;start q case
        cmp byte [buffer],113
        jnz endQCase
        jmp endLabel                         
        endQCase:
        ;end q case


        ; ;start + case  
        ; cmp byte [buffer],43
        ; checkArgs 2                         ;check if there is enough arguments
        ; mov ebx, [stack]                    ;arg0
        ; sub dword stack,4                   ;index--
        ; mov ecx, [stack]                    ;arg1
        ; sub dword stack,4                   ;index--
        ; ;------------------------------------------------------------------do add with the object linked list
        ; add ebx, ecx                        ;ebx=ebx+ecx
        ; ;------------------------------------------------------------------
        ; mov [stack], ebx                    ;stack[index]=ebx
        ; add dword stack, 4                  ;index++
        ; jmp myCalc.loop
        ;end + case

        ; ;compare buffer to "p" 
        ; cmp byte [buffer],112 
        ; checkArgs 1                         ;check if there is enough arguments
        ; mov ebx, [stack]                    ;arg0
        ; sub dword stack,1                   ;index--
        ; ;----------------------------------------
        ; push ebx
        ; ;---------------convert to string/integer
        ; push [stdout]
        ; call printf
        ; add esp, 8
        ; mov stack[index], ebx               ;stack[index]=ebx
        ; inc index                           ;index++

        ; ;


        ; ;enter number  
        ; mov dword[acccumulator],0            ;the accumulator value
        ; AddNewNode                          ;eax = new node

        ; ;add new node to the stack
        ; mov ebx, dword [stack]
	    ; mov dword [ebx], dword eax
	    ; add ebx, dword 4
	    ; mov [stack], dword ebx
        ; mov dword [currentNode],eax
        ; ;finish add new node to the stack

        ; setEbxToLastIndexBuffer             ;ebx=&buffer[buffer.length]

        ; .startMallocLoop:                   ;37211257

        ; ;read first number of iteration
        ; mov eax,[ebx]                       ;eax = '7'
        ; sub eax,48                          ;eax = 7   111
        ; mov [num],eax                       ;num= 7    111
        ; dec ebx                             ;ecx --
        ; ;end read first number
        
        ; ;case that there is q after the first read we want to finish the read
        ; cmp byte [ebx],113               
        ; jnz myCalc.notQafterFirstLetter            ;jump to end of label if there is no q
        ; mov cl,byte[counter]                       ;cl=counter
        ; mov eax,dword[num]                         ;eax=num
        ; shl eax,cl                                 ;shift the number [counter] bits left
        ; mov edx, [acccumulator]                    ;edx=accumulator
        ; add edx, eax                               ;ask peter about word/dword
        ; mov word [acccumulator], 0                 ;reset accumulator
        ; mov ecx,dword [currentNode]    
        ; mov [ecx],dl                               ;node.value = 10101111 --> AF
        ; jmp myCalc.loop                            ;finish iteration of getting number
        ; .notQafterFirstLetter:
        ; ;end case
        ; ;end read the first number of the iteration

        ; ;read second number in the iteration
        ; mov eax,[ebx]                       ;eax = '5'
        ; sub eax,48                          ;eax = 5   101
        ; shr eax , 3                         ;eax = 40  101000
        ; add dword [num], eax                ;num = 57  101111
        ; dec ebx                             ;ecx --

        ; ;case that there is q after the first read we want to finish the read
        ; cmp byte [ebx],113  
        ; jnz myCalc.notQafterSecondLetter
        ; mov ecx,dword[counter]                ;cl=counter
        ; shl dword [num],cl                        ;ask peter about word/dword
        ; mov edx,[acccumulator]
        ; add dword [num], edx                ;ask peter about word/dword
        ; mov dword[acccumulator], 0               ;reset accumulator
        ; mov ecx,dword [currentNode]
        ; mov edx,[num]
        ; mov [ecx], dl                       ;node.value = 10101111 --> AF
        ; jmp myCalc.loop                     ;finish iteration of getting number
        ; .notQafterSecondLetter:
        ; ;end case

        ; cmp dword [counter], 2              ;if (counter == 2)
        ; jnz myCalc.caseNotTwoBytes          ;case that counter isnt 2 jump to resume normal
        ; mov dword [counter], 0              ;reset counter
        ; shl dword [num],2                   ;shift num 2 bytes for adding
        ; mov edx,dword[acccumulator]
        ; add edx,dword [num]                 ;add accumulator to num
        ; mov dword[acccumulator], 0          ;reset accumulator
        ; mov ecx,dword[currentNode]
        ; mov [ecx],dl                        ;node.value = 10101111 --> AF     ****************
        ; jmp myCalc.newNode                  ;finish iteration and start getting new value from user
        ; .caseNotTwoBytes:                   ;end if
        ; ;end read second number of the iteration

        ; mov eax,[ebx]                       ;eax = '2'
        ; sub eax,48                          ;eax = 2   010
        ; shl eax, 6                          ;eax = 200 010000000 
        ; add dword [num], eax                ;num = 257 010101111  ---> 0
        ; mov ecx,dword[counter]
        ; shl dword[num],cl                   ;ask peter about word/dword
        ; mov edx, [acccumulator]
        ; add edx,dword [num]                 ;ask peter about word/dword
        ; mov dword[acccumulator], 0          ;reset edx
        ; mov ecx,dword[currentNode]
        ; mov byte [ecx],dl                   ;node.value = 10101111 --> AF
        ; shr edx,8                           ;shift num 8 bytes to get the leftmost bytes
        ; mov dword [acccumulator],edx
        ; inc dword [counter]                  ;counter ++
        ; dec ebx                             ;ecx --
        ; ;
        ; cmp byte [buffer],113               ;case that we finished read 3 letters and have q on the next char
        ; jnz myCalc.notQafterThirdLetter            ;case that there is no q jump to resumer
        ; mov edx,dword [acccumulator]
        ; cmp edx,0                           ;check if there is left value in the accumulator edx 
        ; jnz myCalc.endEDX0                 ;if there is value do not add proccess
        ; jmp myCalc.loop                     ;else end process and start new loop 
        ; .endEDX0:                            
        ; push 5                              ;create new node
        ; call malloc
        ; add esp, 4                          ;fix esp
        ; mov edx, dword[acccumulator]
        ; mov byte [eax],dl                 ;mov to the node the acccumulator edx value
        ; mov ebx,dword [currentNode]
        ; mov dword [ebx + 1],eax             ;ebx.next=new node ask peter if thats the way to do it
        ; jmp myCalc.loop                     ;start new loop
        ; .notQafterThirdLetter:
        ; ;
        ; .newNode:
        ; ;create new node
        ; push 5                              ;create new node
        ; call malloc
        ; add esp, 4                          ;fix esp pointer
        ; ;end create new node in eax         
        ; ;********* ask peter ****
        ; mov ecx,dword [currentNode]
        ; mov dword [ecx + 1],eax             ;ebx.next=new node ask peter if thats the way to do it
        ; mov dword[currentNode], eax                        ;ebx = eax
        ; jmp myCalc.startMallocLoop
        ; .endLoop:
endLabel:
popad
mov esp,ebp
ret
    