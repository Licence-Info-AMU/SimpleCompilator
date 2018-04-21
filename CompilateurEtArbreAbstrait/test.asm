%include "io.asm"
section .bss
sinput: resb 255
	$a resd 1
	$b resd 1
section .text
global _start
_start:
	call main
	mov eax, 1
	int 0x80
main:
	push ebp
	mov ebp, esp
	mov eax, sinput
	call readline
	mov eax, sinput
	call atoi
	push eax
	pop ebx
	mov [v$a], ebx
	mov eax, sinput
	call readline
	mov eax, sinput
	call atoi
	push eax
	pop ebx
	mov [v$b], ebx
	mov ebx, [v$a]
	push ebx
	mov ebx, [v$b]
	push ebx
	pop ebx
	pop eax
	add eax, ebx
	push eax
	pop eax
	call iprintLF
	pop ebp
	ret
