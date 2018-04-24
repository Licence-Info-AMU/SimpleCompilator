%include "io.asm"
section .bss
sinput: resb 255
section .text
global _start
_start:
	call main
	mov eax, 1
	int 0x80
main:
	push ebp
	mov ebp, esp
	pop ebp
	ret
