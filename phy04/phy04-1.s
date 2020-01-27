.intel_syntax noprefix
.text
.global my_sin

my_sin:
	push rbp
    mov rbp, rsp
	
	movsd xmm1, xmm0 # xmm1 = current term
	movsd xmm2, xmm0
	mulsd xmm2, xmm0 # xmm2 = x^2
	mov eax, 1 # eax = n
	mov ecx, 0 # manages sign: 0 - minus, 1 - plus
	mov edx, 0 # just zero
	movsd xmm4, xmm0 # xmm4 - last sin value
loop:
	mulsd xmm1, xmm2
	inc eax
	cvtsi2sd xmm3, eax
	divsd xmm1, xmm3
	inc eax
	cvtsi2sd xmm3, eax
	divsd xmm1, xmm3
	
	cvtsi2sd xmm5, edx
	comisd xmm1, xmm5
	je  end_my_sin

	cmp ecx, 0
	je	sub_term
	jmp add_term
sub_term:
	subsd xmm0, xmm1
	mov ecx, 1
	jmp loop
add_term:
	addsd xmm0, xmm1
	mov ecx, 0
	jmp loop
	
end_my_sin:
	pop rbp
	ret
