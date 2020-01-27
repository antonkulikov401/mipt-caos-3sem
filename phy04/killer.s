        .intel_syntax noprefix
   	.global main

        .text



my_atoi: 				#  convert str to int 
					#argument is a valid integer in base 10, otherwise undefined behaviour
	push ebp
	mov ebp, esp
	mov eax, [esp + 8]
	
	mov esi, eax
	
_my_atoi_strlen_loop: 		# optimize: read by 4 bytes	
	inc esi
	movb dl, [esi]
	cmp dl, 0
	jne _my_atoi_strlen_loop
	sub esi, 1		# esi points to last element
	
	mov ecx, 0
	mov edx, 1		# edx is power of 10

_my_atoi_main_loop:
	mov ebx, 0
	movb bl, [esi]
	sub bl, 0x30		# 0x30 is asccii code of '0'
	imul ebx,  edx
	add ecx, ebx
	imul edx, 10
	dec esi
	cmp  esi, eax
	jge _my_atoi_main_loop

	mov eax, ecx
	mov esp, ebp
	pop ebp
	ret	

readline: 			# Buffer overflow!
	
	push ebp
	mov ebp, esp
	mov edi, [esp + 8]
	sub esp, 1
	# char -> esp + 0
_readline_readchar:
	mov eax, 3
	mov ebx, 0
	mov ecx, esp
	mov edx, 1
	int 0x80
	movb al, [esp]
	cmp al, 0xa
	jne _readline_next
	mov al, 0

_readline_next:
	mov [edi], ax
	inc edi 

	cmp al, 0 	
	jne _readline_readchar
	
	add esp, 1
	pop ebp
	ret


readint:			# read int from stdin
	push ebp
	mov ebp, esp
	sub esp, 0x100
	push esp
	call readline
	call my_atoi
	add esp, 0x100	
	mov esp, ebp
	pop ebp
	ret

main:                           # read pid from stdin and kill it   
	call readint
	mov ebx, eax		# pid here
	mov eax, 0x25 		# kill (send signal)
	mov ecx, 9		# 9 is SIGKILL
	int 0x80	
	xor eax, eax
	ret


