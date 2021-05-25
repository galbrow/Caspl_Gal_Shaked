;macro that changes eax value and new_node
%macro AddNewNode 0
	push 5
	call malloc
	add esp, 4 
    mov dword[eax],0
    mov dword[eax+1],0
%endmacro

%macro resetAddNumberVals 0
    mov dword[num],0
    mov dword[acccumulator],0
    mov byte[counter],0
%endmacro

%macro printCounter 0
    mov al,byte [counter]
    and eax,0x0000000f
    printDebugOct eax
%endmacro

%macro printAccumulator 0
    mov eax,dword [acccumulator]
    and eax,0x0000000f
    printDebugOct eax
%endmacro

%macro printNum 0
    mov eax,dword[num]
    printDebugOct eax
%endmacro

%macro resetRegs 0
    mov eax,0
    mov ebx,0
    mov ecx,0
    mov edx,0
%endmacro

%macro printDebugOct 1
    push %1
    push debugOct
    call printf
    add esp,8
%endmacro

%macro printStackFull 0
    push errorOutOfBound
    call printf
    add esp,4
%endmacro

%macro printDebugHex 1
    push %1
    push debugHex
    call printf
    add esp,8
%endmacro
%macro printDebugInt 1
    push %1
    push debugInt
    call printf
    add esp,8
%endmacro
%macro printDebugChar 1
    push %1
    push debugChar
    call printf
    add esp,8
%endmacro
%macro printDebugSrinf 1
    push %1
    push debugString
    call printf
    add esp,8
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
    %%getLastIndex:                       ;check if end of string
    cmp byte [ebx], 0x0                
    jz %%endIndex                  
    inc ebx
    jmp %%getLastIndex
    %%endIndex:                           ;end check ecx=buffer length
    dec ebx
    dec ebx
%endmacro

section .rodata
        CALC:   DB "calc:" , 0 , 0         ;format string 
        debugOct: DB "here %o" ,10, 0
        debugHex: DB "here %x" ,10, 0
        debugInt: DB "here %i" ,10, 0
        debugString: DB "here %s" ,10, 0
        debugChar: DB "here %c" ,10, 0


section .bss
    buffer: resb 80                         ;ask peter for resd
    stack: resd 1
    StackFirst: resd 1
    currentNode: resd 1
    new_node: 	resd 1

section .data
    errorOutOfBound db 'Error: Operand Stack Overflow',0xA
    errorOutOfBoundLen equ $-errorOutOfBound
    errorLackOfArgs db 'Error: Insufficient Number of Arguments on Stack',0xA
    errorLackOfArgsLen equ $-errorLackOfArgs
    numOfOperations: dd 0
    maxIndex: dd 0
    index: dd 0
    counter: db 0
    acccumulator: dd 0
    num: dd 0

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
    ;push ebp
    mov ebp, esp	
    ;pushad
    resetRegs
    mov ecx,5
    cmp dword[ebp + 4],1
    jz no_stackSizeInserted
    mov ebx, dword [ebp + 8]              ;ecx= arg[1]
    add ebx, 4
    mov edx,[ebx]
    mov edx,[edx]
    mov ebx,0x0000FFFF
    and edx,ebx
    mov ecx,edx
    sub ecx, 48                         
    no_stackSizeInserted:
    mov dword[maxIndex] ,ecx             ;add maxIndexValue
    myCalc:
        ;init stack
        shl ecx,2
        push ecx
        call malloc
        add esp, 4
        mov [StackFirst], eax
	    mov [stack], dword StackFirst
        ;end init stack

        loop:
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

        ;start x case
        cmp byte [buffer],113
        jnz endQCase
        jmp endLabel                         
        endQCase:
        ;end x case

        ; ; ;compare buffer to "p" 
        ; cmp byte [buffer],112 
        ; jnz endPrintLabel
        ; ;checkArgs 1                        ;check if there is enough arguments
        ; mov ebx, [stack]                    ;arg0
        ; sub dword [stack],4                 ;index--
        ; mov edx,ebx                         ;edx*=ebx
        ; mov ebx,[ebx]                       ;ebx=node
        ; mov dword[edx],0                    ;edx=0 (delete node)
        ; ;----------------------------------------

        ; ;---------------convert to string/integer
        ; and ebx,0x0000FFFF
        ; push ebx
        ; ;push [hex_format]
        ; push dword[stdout]
        ; call fprintf
        ; add esp, 12
        ; ;mov stack[index], ebx               ;stack[index]=ebx
        ; inc word[index]                           ;index++
        ; endPrintLabel:

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


        ; ;


        ;enter number
        resetAddNumberVals
        mov ebx,dword[maxIndex]
        mov eax,dword[stack]
        ;mov eax,[eax]
        mov ecx,dword[StackFirst]
        sub eax,[ecx]
        cmp eax,ebx
        jnz stackIsNotFull
        printStackFull
        jmp loop
        stackIsNotFull:

        AddNewNode                           ;eax = new node
        ;add new node to the stack
        mov ebx, dword [stack]
	    mov dword [ebx], dword eax
	    add ebx, dword 4
	    mov [stack], dword ebx
        mov dword [currentNode],eax
        ;finish add new node to the stack

        setEbxToLastIndexBuffer             ;ebx=&buffer[buffer.length]
        startMallocLoop:                   ;37211257

        ;read first number of iteration
        mov eax,0
        mov eax,[ebx]                       ;eax = '7'
        sub eax,48                          ;eax = 7   111
        mov [num],al                        ;num= 7    111
        dec ebx                             ;ecx --
        ;end read first number


        ;case that we are in the end of buffer after read the first number
        mov edx,ebx
        inc edx
        cmp edx,buffer         
        jnz notQafterFirstLetter                   ;jump to end of label if there is no x
        mov cl,byte[counter]                       ;cl=counter
        and ecx,0x0000000f
        mov eax,dword[num]                         ;eax=num
        shl eax,cl                                 ;shift the number [counter] bits left
        mov edx, [acccumulator]                    ;edx=accumulator
        and edx,0x00000ff
        add edx, eax                               ;ask peter about word/dword
        mov word [acccumulator], 0                 ;reset accumulator
        mov ecx,dword [currentNode]    
        mov [ecx],dl                               ;node.value = 10101111 --> AF
        printDebugHex edx
        resetAddNumberVals
        jmp loop                                   ;finish iteration of getting number
        notQafterFirstLetter:
        ;end case
        ;end read the first number of the iteration

        ;read second number in the iteration
        mov eax,0
        mov eax,[ebx]                       ;eax = '5'
        sub eax,48                          ;eax = 5   101
        shl eax,3                           ;eax = 40  101000 
        add [num], al                       ;num = 57  101111
        dec ebx                             ;ecx --
        ;case that we are in the end of buffer after read the second number
        mov edx,ebx
        inc edx
        cmp edx,buffer  
        jnz notQafterSecondLetter
        mov cl,byte[counter]                  ;cl=counter
        and ecx,0x0000000f
        mov eax, dword[num]
        shl eax,cl                    ;ask peter about word/dword
        mov dword[num],eax
        mov edx,[acccumulator]
        and edx,0x00000ff
        add dword [num], edx                  ;ask peter about word/dword
        mov dword[acccumulator], 0            ;reset accumulator
        mov ecx,dword [currentNode]
        mov edx,dword[num]
        mov [ecx], dl                         ;node.value = 10101111 --> AF
        printDebugHex edx
        resetAddNumberVals
        jmp loop                              ;finish iteration of getting number
        notQafterSecondLetter:
        ;end case

        ;case that we have 2 bits in the accumulator so we finish read after 2 numbers iteration
        cmp byte [counter], 2              ;if (counter == 2)
        jnz caseNotTwoBytes                ;case that counter isnt 2 jump to resume normal
        mov byte [counter], 0              ;reset counter
        shl dword [num],2                  ;shift num 2 bytes for adding
        mov edx,dword[acccumulator]
        and edx,0x000000ff
        add edx,dword [num]                 ;add accumulator to num
        mov dword[acccumulator], 0          ;reset accumulator
        mov ecx,dword[currentNode]
        mov [ecx],dl                        ;node.value = 10101111 --> AF     ****************
        jmp newNode                         ;finish iteration and start getting new value from user
        caseNotTwoBytes:                    ;end if
        ;end read second number of the iteration

        ;read third number
        mov edx,0
        mov edx,[ebx]                       ;eax = '2'
        sub edx,48                          ;eax = 2   010
        shl edx, 6                          ;eax = 200 010000000 
        add [num], dl                       ;num = 257 010101111  ---> 0
        ;end read
        mov cl,byte[counter]
        and ecx,0x000000f
        mov edx,dword[num]
        shl edx,cl     
        ;work              
        mov eax, [acccumulator]
        and eax,0x000000ff
        add edx,eax
        mov ecx,0
        add cl,dl
        mov dword[num],ecx
        mov dword[acccumulator], 0          ;reset edx
        mov ecx,dword[currentNode]
        mov byte [ecx],dl                   ;node.value = 10101111 --> AF
        shr edx,8                           ;shift num 8 bytes to get the leftmost bytes
        and edx,0x0000000f
        mov dword [acccumulator],edx
        ;printDebugHex eax
        inc byte [counter]                  ;counter ++
        mov al,[counter]
        and eax,0x0000000f
        dec ebx                             ;ebx --

        ;case that we finished read 3 letters and have q on the next char
        mov edx,ebx
        inc edx
        cmp edx,buffer               
        jnz notQafterThirdLetter            ;case that there is no x jump to resumer
        mov edx,dword [acccumulator]
        cmp edx,0                           ;check if there is left value in the accumulator edx 
        jnz endEDX0                         ;if there is value do not add proccess
        jmp loop                            ;else end process and start new loop 
        endEDX0:                            
        AddNewNode                          
        mov edx, dword[acccumulator]
        mov byte [eax],dl                  ;mov to the node the acccumulator edx value
        mov ebx,dword [currentNode]
        mov dword [ebx + 1],eax            ;ebx.next=new node ask peter if thats the way to do it
        jmp loop                           ;start new loop
        notQafterThirdLetter:
        ;end case

        ;insert new node to list
        newNode:
        AddNewNode
        mov ecx,dword [currentNode]
        mov dword [ecx + 1],eax             ;ebx.next=new node ask peter if thats the way to do it
        mov dword[currentNode], eax         ;ebx = eax
        mov edx,dword[stack]
        ;end insert new node

        printDebugHex dword[num]
        ;repeat
        jmp startMallocLoop
        endLoop:
endLabel:
mov esp,ebp
ret
    