.intel_syntax noprefix
.text
.global _start

_start:
	mov eax, 45 # brk syscall
    mov ebx, 0
    int 0x80
    mov [tmp], eax
    mov eax, 45
    mov ebx, [tmp]
    add ebx, 65011712 # allocate 62mb
    int 0x80
	mov esi, 1 # number of all chars
	mov edi, 1 # flag for first line

	mov ecx, [tmp]
	movb [ecx], '\n'
	inc ecx
	mov [tmp], ecx
read_loop:
	mov eax, 3
	mov ebx, 0
	mov ecx, [tmp]
	mov edx, 65011712
	int 0x80

	cmp eax, 0
	jne	no_write
	sub	word ptr [tmp], 1
	jmp add_lf

	no_write:
	mov ecx, [tmp]
	add ecx, eax
	mov [tmp], ecx
	add esi, eax

	jmp read_loop

add_lf:
	mov eax, [tmp]
	mov al, [eax]
	cmp al, '\n'
	jne write
	sub word ptr [tmp], 1
	sub esi, 1
write:
	mov edx, 0
	cmp esi, 0
	jle end
find_length:
#		mov ecx, [tmp]
   # 	dec ecx
   # 	mov [tmp], ecx
	mov eax, [tmp]
	movb al, [eax]
	inc edx
    mov ecx, [tmp]
    dec ecx
    mov [tmp], ecx
	cmp al, '\n'
	je	write_line
	jmp find_length
write_line:
	sub esi, edx
	mov ecx, [tmp]
	inc ecx
	cmp edi, 1
	je	add_extra
write_call:
	mov eax, 4
    mov ebx, 1
	int 0x80
	jmp write

add_extra:
	inc ecx
	sub edx, 1
	mov edi, 0
	jmp write_call


end:
	mov eax, [tmp]
	inc eax
	mov [tmp], eax
	movb [eax], '\n'
	mov eax, 4
	mov ebx, 1
	mov ecx, [tmp]
	mov edx, 1
	int 0x80
	mov eax, 1
	mov ebx, 0
	int 0x80


.data
tmp: .word 0

