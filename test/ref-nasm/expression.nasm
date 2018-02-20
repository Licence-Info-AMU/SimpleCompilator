%include	'io.asm'

section	.bss
sinput:	resb	255	;reserve a 255 byte space in memory for the users input string
v$a:	resd	1
v$b:	resd	1

section	.text
global _start
_start:
	call	main
	mov	eax, 1		 ; 1 est le code de SYS_EXIT
	int	0x80		 ; exit
main:
	push	ebp		 ; sauvegarde la valeur de ebp
	mov	ebp, esp		 ; nouvelle valeur de ebp
	mov	eax, sinput
	call	readline
	mov	eax, sinput
	call	atoi
	push	eax
	pop	ebx
	mov	[v$a], ebx		 ; stocke registre dans variable
	mov	eax, sinput
	call	readline
	mov	eax, sinput
	call	atoi
	push	eax
	pop	ebx
	mov	[v$b], ebx		 ; stocke registre dans variable
	mov	ebx, [v$a]		 ; lit variable dans ebx
	push	ebx
	mov	ebx, [v$b]		 ; lit variable dans ebx
	push	ebx
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	add	eax, ebx		 ; effectue l'opération
	push	eax		 ; empile le résultat
	pop	eax
	call	iprintLF
	pop	ebp		 ; restaure la valeur de ebp
	ret
