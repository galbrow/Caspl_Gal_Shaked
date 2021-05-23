section .rodata
        an: dd 0 

section .bss

section .data
        format: DB "the calculated number is: %d", 10, 0
section .text
        global assFunc ;
        extern c_checkValidity
        extern printf

assFunc:
        push ebp                 ; save base pointer original value
        mov ebp, esp             ; use base pointer to access stack contents assFunc AF
        sub esp,4                ; space for local variable
        mov ebx, [ebp+8]         ; ebx= first arg of assFunc
;-------------------------------------------------------------------------------------
        push ebx                 ; push argument to stack 
        call c_checkValidity     ; call check validity func
        sub eax, 48              ; check if char is 0
        je equals_zero           ; case char=0 jump to equals_zero and else continue
        shl ebx,2                ; z=x*4
        jmp endIf                ; continue
        equals_zero:             
        shl ebx,3                ; z=x*3
        endIf:                   ; end label
        push ebx                 ; push result to the stack (second arg for printf)
        push format              ; push the format (first arg for printf)
        call printf              ; calls c printf
        mov eax,0                ; return value
        mov esp,ebp              ; get back to the beginning of the AF
        pop ebp                  ; pops the AF of the prev function
        ret                      ; end of func