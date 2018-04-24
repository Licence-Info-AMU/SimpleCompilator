%include "io.asm"
section .bss
sinput: resb 255
section .text
global _start
_start:
	call main
	mov eax, 1
	int 0x80
add:
	push ebp
	mov ebp, esp
	mov ebx, [ebp + 12]
	push ebx
	mov ebx, [ebp + 8]
	push ebx
	pop ebx
	pop eax
	add eax, ebx
	push eax
	pop  eax
	mov [ebp + 16], eax
	pop ebp
	ret
	pop ebp
	ret
main:
	push ebp
	mov ebp, esp
	sub esp, 12
	push 1
	pop ebx
	mov [ebp - 4], ebx
	push 2
	pop ebx
	mov [ebp - 8], ebx
	push 3
	pop ebx
	mov [ebp - 12], ebx
