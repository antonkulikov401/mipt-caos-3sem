.intel_syntax noprefix
.text
.global _start

_start:
	call allocate
	mov ebp, esp

read:
	mov edi, 0 # current string len
read_loop:
	mov eax, 3
	mov ebx, 0
	mov ecx, [tmp]
	mov edx, 1
	int 0x80
	cmp eax, 0x0
	je push_line
	inc edi
	
	mov ecx, [tmp]
    inc ecx
    mov [tmp], ecx
	dec ecx
	movb cl, [ecx]
	cmp cl, '\n'
	je	push_line
	
	#mov eax, [tmp]
	#inc eax
	#mov [tmp], eax
	jmp read_loop
push_line:
	cmp edi, 0 # if we haven't read anything
	je	complete_final_string
	push edi
	jmp read

complete_final_string:
	mov ecx, [tmp]
	movb cl, [ecx]
	cmp cl, '\n'
	jne add_line_break
	jmp write
add_line_break:
	mov eax, 1
	int 0x80
	#mov ecx, [tmp]
	#inc ecx
	#movb [ecx], '\n'
	#pop ecx
	#inc ecx
	#push ecx
write:
	cmp ebp, esp
	je	end
	pop edi
	mov eax, 4
	mov ebx, 1
	mov ecx, [tmp]
	sub ecx, edi
	#inc ecx
	mov [tmp], ecx
	mov edx, edi
	int 0x80
	jmp write

end:
	mov eax, 1
	mov ebx, 0
	#int 0x80

allocate:
	enter 0, 0
	push ebx

	mov eax, 45 # brk syscall
	mov ebx, 0
	int 0x80
	mov [tmp], eax
	mov eax, 45
	mov ebx, [tmp]
	add ebx, 33554432 # allocate 32mb
	int 0x80

	pop ebx
	leave
	ret

.data
tmp: .word 0
