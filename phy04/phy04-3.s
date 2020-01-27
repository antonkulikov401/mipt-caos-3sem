.intel_syntax noprefix
.text
.global dot_product

dot_product:
	enter 0, 0
	
	mov esi, [ebp + 12]
	mov edi, [ebp + 16]
	mov ecx, [ebp + 8]
	mov eax, 0 # counter
	cvtsi2sd xmm0, eax

loop:
	cmp eax, ecx
	jge end_loop
	movaps xmm1, [esi + 4 * eax]
	movaps xmm2, [edi + 4 * eax]
	dpps xmm1, xmm2, 0xF1
	addps xmm0, xmm1
	add eax, 4
	jmp loop

end_loop:
	sub esp, 4
	movss [esp], xmm0
	fld dword ptr [esp]
	add esp, 4
	leave
	ret

