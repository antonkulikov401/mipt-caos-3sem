.intel_syntax noprefix
.text
.global summ

summ:
	enter 0, 0 # push ebp; mov ebp, esp
	push ebx
	mov edi, [ebp + 20] # R
	mov eax, [ebp + 16] # A
	mov esi, [ebp + 12] # B
	mov ebx, 0 # counter

loop:
	cmp ebx, [ebp + 8]
	jge end_loop
	mov ecx, [eax + ebx * 4]
	mov edx, [esi + ebx * 4]
	add ecx, edx
	mov [edi + ebx * 4], ecx
	add ebx, 1
	jmp loop

end_loop:
	pop ebx
	leave # pop ebp
	ret

