section	.rodata				; we define (global) read-only variables in .rodata section
	format_string: db "%s", 10, 0	; format string

section .bss				; we define (global) uninitialized variables in .bss section
	an: resb 128			; enough to store integer in [-2,147,483,648 (-2^31) : 2,147,483,647 (2^31-1)]

section .text
	global convertor
	extern printf

convertor:
	push ebp
	mov ebp, esp	
	pushad			
	mov ecx, dword [ebp+8]	; get function argument (pointer to string)
	mov edx, an
	startlabel:				; label to start
	cmp byte [ecx],10		; if ecx='\n'
	jz endlabel				; jump
	cmp [ecx],byte 58		; char is number
	jb startCalc			; jump
	sub byte[ecx],7			; do 
	startCalc:				
	add edx,3				; get to the 4 bit of the char
	mov byte [edx],49		; set char = 1
	shr byte [ecx], 1		; shift right (check the righmost char)
	jb endbit1				; jump
	mov byte [edx],48		; char=0
	endbit1:
	dec edx					; edx pointer --
;--------------------------------------------------------------------------
	mov byte [edx],49		; char =1
	shr byte [ecx], 1		; shift right (check the righmost char)
	jb endbit2				; jump
	mov byte [edx],48		; char=0
	endbit2:
	dec edx					; edx pointer --
;------------------------------------------------------------------------------
	mov byte [edx],49		; char =1
	shr byte [ecx], 1		; shift right (check the righmost char)
	jb endbit3				; jump
	mov byte [edx],48		; char = 0
	endbit3:
	dec edx					; edx pointer --
;--------------------------------------------------------------------------------
	mov byte [edx],49		; char =1
	shr byte [ecx], 1		; shift right (check the righmost char)
	jb endbit4				; jump
	mov byte [edx],48		; char = 0
	endbit4:
;--------------------------------------------------------------------------------------
	add edx,4				; get to the rightmost char
	inc ecx					; get string[i+1]
	jmp startlabel			; begin loop again
	endlabel:				; endlabel
	mov byte[edx],0
	
	end:
	push an					; call printf with 2 arguments -  
	push format_string		; pointer to str and pointer to format string
	call printf
	add esp, 8				; clean up stack after call

	popad			
	mov esp, ebp	
	pop ebp
	ret