%include	'io.asm'

section	.bss
sinput:	resb	255	;reserve a 255 byte space in memory for the users input string

section	.text
global _start
_start:
	call	main
	mov	eax, 1		 ; 1 est le code de SYS_EXIT
	int	0x80		 ; exit
max:
	push	ebp		 ; sauvegarde la valeur de ebp
	mov	ebp, esp		 ; nouvelle valeur de ebp
	mov	ebx, [ebp + 12]		 ; lit variable dans ebx
	push	ebx
	mov	ebx, [ebp + 8]		 ; lit variable dans ebx
	push	ebx
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	cmp	eax, ebx
	jl	e2
	push	0
	jmp	e3
e2:
	push	1
e3:
	pop	eax
	cmp	eax, 00
	jz	e1
	mov	ebx, [ebp + 8]		 ; lit variable dans ebx
	push	ebx
	pop	eax
	mov	[ebp + 16], eax		 ; ecriture de la valeur de retour
	pop	ebp		 ; restaure la valeur de ebp
	ret
e1:
	mov	ebx, [ebp + 12]		 ; lit variable dans ebx
	push	ebx
	pop	eax
	mov	[ebp + 16], eax		 ; ecriture de la valeur de retour
	pop	ebp		 ; restaure la valeur de ebp
	ret
	pop	ebp		 ; restaure la valeur de ebp
	ret
main:
	push	ebp		 ; sauvegarde la valeur de ebp
	mov	ebp, esp		 ; nouvelle valeur de ebp
	sub	esp, 8	; allocation variables locales
	mov	eax, sinput
	call	readline
	mov	eax, sinput
	call	atoi
	push	eax
	pop	ebx
	mov	[ebp - 4], ebx		 ; stocke registre dans variable
	mov	eax, sinput
	call	readline
	mov	eax, sinput
	call	atoi
	push	eax
	pop	ebx
	mov	[ebp - 8], ebx		 ; stocke registre dans variable
	sub	esp, 4		 ; allocation valeur de retour
				; empile arg 0
	mov	ebx, [ebp - 4]		 ; lit variable dans ebx
	push	ebx
				; empile arg 1
	mov	ebx, [ebp - 8]		 ; lit variable dans ebx
	push	ebx
	call	max
	add	esp, 8		; desallocation parametres
	mov	ebx, [ebp - 4]		 ; lit variable dans ebx
	push	ebx
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	cmp	eax, ebx
	je	e6
	push	0
	jmp	e7
e6:
	push	1
e7:
	pop	eax
	cmp	eax, 00
	jz	e4
	mov	ebx, [ebp - 4]		 ; lit variable dans ebx
	push	ebx
	pop	eax
	call	iprintLF
	jmp	e5
e4:
	mov	ebx, [ebp - 8]		 ; lit variable dans ebx
	push	ebx
	pop	eax
	call	iprintLF
e5:
	add	esp, 8	; desallocation variables locales
	pop	ebp		 ; restaure la valeur de ebp
	ret
