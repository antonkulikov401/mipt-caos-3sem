.intel_syntax noprefix
.text
.global _start

_start:
	mov eax, 3
	mov ebx, 0
	mov ecx, offset buff
	mov edx, 1
	int 0x80
	
	cmp eax, 0x0
	je	end

	mov eax, 4
	mov ebx, 1
	mov ecx, offset buff
	mov edx, 1
	int 0x80

	jmp _start

end:
	mov eax, 1
	int 0x80

.data
buff: .byte 0
