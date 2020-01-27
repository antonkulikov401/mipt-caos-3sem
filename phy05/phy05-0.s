.intel_syntax noprefix
.text
.global _start

_start:
	mov eax, 4 # 4 - write
	mov ebx, 1 # 1 - stdout
	mov ecx, offset str
	mov edx, 13
	int 0x80
	mov eax, 1
	int 0x80

.data
str: 
	.asciz "Hello, World!"

