        .intel_syntax noprefix
   	.global main

        .text
main:                               
	
	mov eax, 4
	mov ebx, 1
	mov ecx, offset hwStr
	mov edx, 15
	int 0x80 // interrupt, 0x80 means syscal
			 // eax - number of syscall, bcd - arguments
	xor eax, eax
	ret

	.data
hwStr:
	.asciz "Hello, world!\n"

