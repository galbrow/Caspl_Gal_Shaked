;macro that changes eax value and new_node
%macro AddNewNode 0
    pushad
    push 1
	push 5
	call calloc
	add esp, 8
    mov dword[new_node],eax
    popad
    mov eax,dword[new_node]
%endmacro

%macro clearBuffer 0
    pushad
    mov eax,buffer
    mov ebx,79
    cmp ebx, -1
    jz %%endmacro
    mov byte[ebx+eax],0
    dec ebx
    %%endmacro:
    popad
%endmacro

%macro printBN 0
    pushad
    push bn
    call printf
    add esp,4
    popad
%endmacro

%macro printOct 1
    pushad
    push %1
    push OctFormat
    call printf
    add esp,8
    popad
%endmacro

;pops node to ecx
%macro popNodeFromStack 0
    pushad
    resetRegs
    mov edx, 0
    mov dl, [index]
    cmp edx, 0
    jnz %%stackIsEmpty
    mov eax, 0
    printStackEmpty
    jmp loop
    %%stackIsEmpty:
    dec byte [index]
    sub edx,1
    mov ebx, [stack]
    mov ecx, [ebx + edx * 4]
    mov dword [ebx + edx * 4] , 0
    mov dword[new_node],0
    mov dword[new_node],ecx
    popad
    mov ecx,0
    mov ecx,dword[new_node]
%endmacro

%macro resetAddNumberVals 0
    mov dword[num],0
    mov dword[acccumulator],0
    mov byte[counter],0
%endmacro

%macro printCounter 0
    mov al,byte [counter]
    and eax,0x000000ff
    printDebugOct eax
%endmacro

%macro printAccumulator 0
    mov eax,dword [acccumulator]
    and eax,0x000000ff
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
    pushad
    push %1
    push debugOct
    call printf
    add esp,8
    popad
%endmacro

%macro printStackFull 0
    pushad
    push errorOutOfBound
    call printf
    add esp,4
    popad
%endmacro

%macro printStackEmpty 0
    pushad
    push errorLackOfArgs
    call printf
    add esp,4
    popad
%endmacro

%macro printDebugHex 1
    pushad
    push %1
    push debugHex
    call printf
    add esp,8
    popad
%endmacro

%macro printDebugInt 1
    pushad
    push %1
    push debugInt
    call printf
    add esp,8
    popad
%endmacro

%macro printDebugChar 1
    pushad
    push %1
    push debugChar
    call printf
    add esp,8
    popad
%endmacro

%macro printDebugSrinf 1
    pushad
    push %1
    push debugString
    call printf
    add esp,8
    popad
%endmacro

%macro addNewNodeToStack 0
    pushad
    mov ebx,0
    mov ecx,0
    mov edx,0
    mov dl,[index]
    mov ebx, dword [stack]
    mov [edx*4+ebx],eax
    mov dword [currentNode],eax
    inc byte[index]
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
    CharFormat: DB "%c" ,0, 0
    OctFormat: DB "%o" ,0, 0
    bn: DB "" ,10, 0
    errorOutOfBound: db "Error: Operand Stack Overflow",10,0
    errorLackOfArgs: db "Error: Insufficient Number of Arguments on Stack",10,0

section .bss
    buffer: resb 80                         
    stack: resd 1
    StackFirst: resd 1
    currentNode: resd 1
    new_node: 	resd 1
    first_node: resd 1
    second_node: resd 1

section .data
    numOfOperations: dd 0
    maxIndex: db 0
    index: db 0
    counter: db 0
    acccumulator: dd 0
    num: dd 0
    printCount: dd 0

section .text
  align 16
  global main
  extern printf
  extern fprintf 
  extern malloc
  extern calloc 
  extern free 
  extern fgets 
  extern stdout
  extern stdin
  extern stderr

main:
    push ebp
    mov ebp, esp	
    resetRegs
    mov ecx,5
    cmp dword[ebp + 8],1
    jz no_stackSizeInserted
    mov ebx, dword [ebp + 12]              ;ecx= arg[1]
    add ebx, 4
    mov edx,[ebx]
    mov edx,[edx]
    mov ebx,0x0000FFFF
    and edx,ebx
    mov ecx,0
    mov ecx,edx
    sub ecx, 48                         
    no_stackSizeInserted:
    mov [maxIndex] ,cl                   ;add maxIndexValue
    myCalc:
        ;init stack
        shl ecx,2
        push ecx
        call malloc
        add esp, 4
	    mov [stack], eax
        ;end init stack

        loop:
        ;print calc:
        push CALC
        call printf
        add esp, 4                          ;print calc:
        ;end print calc:
        
        clearBuffer
        ;get input from user
        push dword [stdin]
        push dword 80
        push dword buffer
        call fgets
        add esp, 12                         ;buffer = fgets
        ;end get input from user

        ;start q case
        cmp byte [buffer],113
        jnz endQCase
        jmp endLabel                         
        endQCase:
        ;end x case

        ;need to fix cases that the number has more than 8 digits
        ;compare buffer to "p" 
        cmp byte [buffer],112 
        jnz endPrintLabel
        mov dword[printCount],0
        clearBuffer
        resetRegs
        popNodeFromStack
        inc dword[numOfOperations]
        startPrint:
        mov ebx,0
        mov edx,0
        mov eax,0

        ;read first             edx=accumulator, ecx=currentNode
        cmp ecx,0
        jz printNewLine          ;case of last index
        mov edx,[ecx]            ;ecx=poped node
        and edx,0x000000ff
        add ebx,edx
        mov edx,[ecx+1]
        mov ecx,[ecx+1]
        cmp ecx,0
        jz printNewLine 
        mov edx,[edx]
        mov eax,edx
        and eax,0x000000ff
        shl eax,8
        add ebx,eax

        mov edx,[ecx+1]
        mov ecx,[ecx+1]
        cmp ecx,0
        jz printNewLine 
        mov edx,[edx]
        mov eax,edx
        and eax,0x000000ff
        shl eax,16
        add ebx,eax

        ;printOct ebx

        mov ecx,[ecx+1]
        cmp ecx,0
        jz printNewLine

        pushad
        inc dword[printCount]
        mov ecx,buffer
        add ecx,79
        mov edx,0
        mov edx,dword[printCount]
        sub ecx,edx
        sub ecx,edx
        sub ecx,edx
        mov dword[ecx],ebx
        ;printDebugOct ebx
        popad

        jmp startPrint
        printNewLine:
        printOct ebx
        mov edx,0
        mov edx,dword[printCount]
        cmp edx,0
        jz endPrint
        printNewLineWithoutOct:
        mov ebx,buffer
        add ebx,79
        cmp edx,0
        jz endPrint
        mov ecx,0
        mov ecx,edx
        sub ebx,ecx
        sub ebx,ecx
        sub ebx,ecx
        mov eax,dword[ebx]
        shl eax,8
        shr eax,8
        printOct eax
        dec edx
        jmp printNewLineWithoutOct
        endPrint:
        printBN
        jmp loop
        endPrintLabel:

        ;start n case
        cmp byte [buffer],110
        jnz endNLabel
        resetRegs
        popNodeFromStack
        inc dword[numOfOperations]
        beginNloop:
        mov ecx,[ecx+1]
        cmp ecx,0
        jz lastNode
        inc edx 
        jmp beginNloop
        lastNode:
        inc edx
        and edx,0x000000ff
        ;printDebugOct edx
        AddNewNode
        addNewNodeToStack
        mov [eax],dl
        mov edx,0
        jmp loop
        endNLabel:

        ;start & case
        cmp byte [buffer],38
        jnz endAndCase
        cmp byte[index],2
        jnb caseOf2Ops
        printStackEmpty
        jmp loop
        caseOf2Ops:
        inc dword[numOfOperations]
        popNodeFromStack
        mov eax,0
        mov edx,dword[stack]
        mov al,byte [index]
        dec al
        mov edx,[edx+4*eax]
        andLoop:
        mov al,byte[ecx]
        and byte[edx],al
        inc edx
        inc ecx
        cmp dword[edx],0
        jz loop
        cmp dword[ecx],0
        jnz caseNotEmpty
        mov dword[edx],0
        jmp loop
        caseNotEmpty:
        jmp andLoop
        endAndCase:


        ;start + case  
        cmp byte [buffer],43   ;edx=overflow
        jnz endPlus
        resetRegs
        cmp byte[index],2
        jnb caseOf2OpsPlus
        printStackEmpty
        jmp loop
        caseOf2OpsPlus:
        ;-------------------do add with the object linked list
        popNodeFromStack
        mov dword[first_node],ecx
        popNodeFromStack
        mov dword[second_node],ecx
        AddNewNode
        addNewNodeToStack
        mov edx,0
        startPlusLoop:
        mov ebx,dword[first_node]
        mov ecx,dword[second_node]
        mov ebx,dword[ebx]
        mov ecx,dword[ecx]
        and ebx,0x000000ff
        and ecx,0x000000ff
        add edx,ebx
        add edx,ecx
        mov ebx,dword[first_node]
        mov ecx,dword[second_node]
        mov ecx,dword[ecx+1]
        mov byte[eax],dl
        shr edx,8
        mov ebx,dword[first_node]
        mov ecx,dword[second_node]
        mov ebx,dword[ebx+1]
        mov ecx,dword[ecx+1]
        mov dword[first_node],ebx
        mov dword[second_node],ecx
        cmp ecx,0
        jz addRest
        cmp ebx,0
        jnz continueCase
        mov ebx,ecx
        mov ecx,0
        jmp addRest
        continueCase:
        mov ebx,eax
        AddNewNode
        mov dword[ebx+1],eax
        jmp startPlusLoop
        ;------------------------------------------------------------------
        addRest: ;ebx have more values
        cmp ebx,0
        jnz only1Null
        cmp dl,0
        jz loop
        mov ecx,eax
        AddNewNode
        mov byte[eax],dl
        mov [ecx+1],eax
        mov edx,0
        jmp loop
        only1Null:
        cmp dl,0
        jnz cont
        mov ebx,[ebx+1]
        mov dword[eax+1],ebx
        jmp loop
        cont:
        ;printDebugOct edx
        mov ecx,0
        mov cl,byte[ebx]
        add edx,ecx
        mov byte[eax],dl
        shr edx,8
        mov ecx,eax
        AddNewNode
        mov dword[ecx+1],eax
        jmp addRest
        endPlus:
        ;end + case
        


        ;enter number
        resetAddNumberVals
        resetRegs
        clearBuffer
        mov al,[index]
        cmp [maxIndex],al
        jnz stackIsNotFull
        printStackFull
        jmp loop
        stackIsNotFull:

        AddNewNode                           ;eax = new node
        addNewNodeToStack
        ;finish add new node to the stack
        resetRegs
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
        and ecx,0x000000ff
        mov eax,dword[num]                         ;eax=num
        shl eax,cl                                 ;shift the number [counter] bits left
        mov edx, [acccumulator]                    ;edx=accumulator
        and edx,0x000000ff
        add edx, eax                               ;ask peter about word/dword
        mov ecx,dword [currentNode]    
        mov [ecx],dl                               ;node.value = 10101111 --> AF
        mov ecx,[ecx]
        printDebugHex ecx
        resetAddNumberVals
        resetRegs
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
        and ecx,0x000000ff
        mov eax, dword[num]
        shl eax,cl                    ;ask peter about word/dword
        mov dword[num],eax
        mov edx,[acccumulator]
        and edx,0x000000ff
        add dword [num], edx                  ;ask peter about word/dword
        mov dword[acccumulator], 0            ;reset accumulator
        mov ecx,dword [currentNode]
        mov edx,dword[num]
        mov [ecx], dl                         ;node.value = 10101111 --> AF
        mov ecx,[ecx]
        and ecx,0x000000ff
        printDebugHex ecx
        resetAddNumberVals
        resetRegs
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
        and edx,0x000000ff
        sub edx,48                          ;eax = 2   010
        shl edx, 6                          ;eax = 200 010000000 
        add [num], edx                       ;num = 257 010101111  ---> 0
        ;end read
        mov cl,byte[counter]
        and ecx,0x000000ff
        mov edx,dword[num]
        shl edx,cl     
        ;work              
        mov eax, dword[acccumulator]
        and eax,0x000000ff
        add edx,eax
        mov ecx,0
        add ecx,edx
        and ecx,0x000000ff
        mov dword[num],ecx
        mov ecx,dword[currentNode]
        mov byte [ecx],dl                   ;node.value = 10101111 --> AF
        shr edx,8                           ;shift num 8 bytes to get the leftmost bytes
        mov dword [acccumulator],edx
        ;printDebugHex eax
        ;printAccumulator
        inc byte [counter]                  ;counter ++
        mov al,[counter]
        and eax,0x000000ff
        dec ebx                             ;ebx --

        ;case that we finished read 3 letters and have q on the next char
        mov edx,ebx
        inc edx
        cmp edx,buffer               
        jnz notQafterThirdLetter            ;case that there is no x jump to resumer
        mov edx,dword [acccumulator]
        cmp edx,0                           ;check if there is left value in the accumulator edx 
        jnz endEDX0                         ;if there is value do not add proccess
        printDebugHex dword[num]
        jmp loop                            ;else end process and start new loop 
        endEDX0:                            
        AddNewNode                          
        mov edx, dword[acccumulator]
        and edx,0x000000ff
        mov byte [eax],dl                  ;mov to the node the acccumulator edx value
        mov ebx,dword [currentNode]
        mov dword [ebx + 1],eax            ;ebx.next=new node ask peter if thats the way to do it
        printDebugHex edx
        jmp loop                           ;start new loop
        notQafterThirdLetter:
        ;end case

        ;insert new node to list
        newNode:
        AddNewNode
        mov ecx,dword [currentNode]
        mov dword [ecx+1],eax               ;ebx.next=new node ask peter if thats the way to do it
        mov dword[currentNode], eax         ;ebx = eax
        ;end insert new node
        
        printDebugHex dword[num]
        mov dword[num],0
        ;repeat
        jmp startMallocLoop
        endLoop:
endLabel:
mov eax,0
mov esp,ebp
mov eax,dword[numOfOperations]
ret

        ; ;need to fix cases that the number has more than 8 digits
        ; ;compare buffer to "p" 
        ; cmp byte [buffer],112 
        ; jnz endPrintLabel
        ; resetRegs
        ; popNodeFromStack
        ; inc dword[numOfOperations]
        ; startPrint:
        ; mov ebx,0
        ; mov edx,0
        ; mov eax,0
        ; ;read first 

        ; mov edx,[ecx]
        ; cmp ecx,0
        ; jz printNewLine 
        ; and edx,0x000000ff
        ; add ebx,edx

        ; inc ecx
        ; mov edx,[ecx]
        ; mov ecx,[ecx]
        ; cmp ecx,0
        ; jz printNewLine 
        ; mov edx,[edx]
        ; mov eax,edx
        ; and eax,0x000000ff
        ; shl eax,8
        ; add ebx,eax

        ; inc ecx
        ; mov edx,[ecx]
        ; mov ecx,[ecx]
        ; cmp ecx,0
        ; jz printNewLine 
        ; mov edx,[edx]
        ; mov eax,edx
        ; and eax,0x000000ff
        ; shl eax,16
        ; add ebx,eax
        ; printOct ebx

        ; inc ecx
        ; mov ecx,[ecx]
        ; cmp ecx,0
        ; jz printNewLineWithoutOct

        ; jmp startPrint
        ; printNewLine:
        ; printOct ebx
        ; printNewLineWithoutOct:
        ; printBN
        ; jmp loop
        ; endPrintLabel:



    