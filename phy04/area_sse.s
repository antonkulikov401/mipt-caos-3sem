        .intel_syntax noprefix
   	.global main

        .text
main:                               
	sub esp, 16
	// word radius -> esp + 0
	// qword pi   -> esp + 4
	// qword area     -> esp + 8

	push esp
	push offset formatIn
	call    scanf                   
       	add esp, 4
	pop eax
		
	movss xmm0, [eax] // ss - working with single float, ds - with double

	fldpi // push pi
	fstp dword ptr [esp + 4]		
	movss xmm1, [esp + 4]

	mulss xmm0, xmm0
	mulss xmm0, xmm1
	
	movss [esp + 8], xmm0 // xmm - 128 bit, vector float registers
	fld dword ptr [esp + 8] // load float
	fst qword ptr [esp + 8] // get double
	
	push [esp + 12]
	push [esp + 8]
	push offset formatOut
	call printf
	add esp, 12

	add esp, 16 	
	xor eax, eax
	ret

	.data
formatIn:
	.asciz "%f"
formatOut:
	.asciz "area is %f\n"

