.intel_syntax noprefix
.text
.global _start

_start:
	mov ebp, esp
read:
	mov esi, 0
	mov edi, offset tmp
read_loop:
	mov eax, 3
	mov ebx, 0
	mov ecx, edi
	mov edx, 1
	int 0x80
	
	movb cl, [edi]
	cmp cl, '\n'
	je  push_line

	cmp eax, 0x0
	je push_line
	
	inc esi
	inc edi
	jmp read_loop

push_line:
	cmp esi, 0
	je	write_loop
	cmp edi, offset tmp
	jl	check_eof # jle read
	pushw [edi]
	dec edi
	jmp push_line

check_eof:
	cmp eax, 0x0
	je	write_loop
	jmp read

write_loop:
	cmp ebp, esp
	je	end
	mov eax, 4
	mov ebx, 1
	mov ecx, esp
	mov edx, 1
	int 0x80
	popw di
	dec esi
	jmp write_loop

end:
	mov eax, 1
	int 0x80

.data
tmp: .space 262144
