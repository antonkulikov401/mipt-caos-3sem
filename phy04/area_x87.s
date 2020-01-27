        .intel_syntax noprefix
   	.global main

        .text
main:                               
	sub esp, 12
	// dword radius -> esp + 0
	// qword area -> esp + 4
	mov eax, esp

	push eax
	push offset formatIn
	call    scanf                   
       	add esp, 4
	pop eax
		
	fld dword ptr [eax]
	fld dword ptr [eax]
	
	fldpi

	fmulp
	fmulp
	
	fstp qword ptr [esp + 4]		
	
	push [esp + 8]
	push [esp + 4]
	push offset formatOut
	call printf
	add esp, 12

	add esp, 12 	
	xor eax, eax
	ret

	.data
formatIn:
	.asciz "%f"
formatOut:
	.asciz "area is %f\n"

