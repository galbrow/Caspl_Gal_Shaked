section .data                    	; we define (global) initialized variables in .data section
        an: dd 0              		; an is a local variable of size double-word, we use it to count the string characters

section .text                    	; we write code in .text section
        global do_Str          		; 'global' directive causes the function do_Str(...) to appear in global scope

do_Str:                        		; do_Str function definition - functions are defined as labels
        push ebp              		; save Base Pointer (bp) original value
        mov ebp, esp         		; use Base Pointer to access stack contents (do_Str(...) activation frame)
        pushad                   	; push all signficant registers onto stack (backup registers values)
        mov ecx, dword [ebp+8]		; get function argument on stack
                 			; now ecx register points to the input string
        mov ebx ,0                      ;
        yourCode:			; use label to build a loop for treating the input string characters
                cmp byte [ecx], 32      ; compare the byte on the string to space (if byte != ' ')
                jnz endIf1              ; if not space continue
                inc word [an]           ; an=an+1
        endIf1;                         ; label end of if
                cmp byte [ecx], 65      ; check if the byte is bigger than A
                jb endIf2               ; if byte < A
                cmp byte[ecx],90        
                ja endIf2               ; if byte > Z
                add byte[ecx], 32       ; change byte from upper to lower
        endIf2;
		inc ecx      	    	; increment ecx value; now ecx points to the next character of the string
		cmp byte [ecx], 0   	; check if the next character (character = byte) is zero (i.e. null string termination)
		jnz yourCode      	; if not, keep looping until meet null termination character
        popad                    	; restore all previously used registers
        mov eax,[an]         		; return an (returned values are in eax)
        mov esp, ebp			; free function activation frame
        pop ebp				; restore Base Pointer previous value (to returnt to the activation frame of main(...))
        ret				; returns from do_Str(...) function
