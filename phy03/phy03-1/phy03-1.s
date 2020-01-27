.data
A: .word 0
B: .word 0
scanf_fmt: .asciz "%d %d"
printf_fmt: .asciz "%d\n"

.text
.global main

main:
	push {lr}

	ldr	r0, =scanf_fmt
	ldr r1, =A
	ldr r2, =B
	bl	scanf

	ldr r0, =printf_fmt
	ldr r1, =A
	ldr r1, [r1] // r1 = A
	ldr r2, =B
	ldr r2, [r2] // r2 = B
	add r1, r1, r2 // r1 = A+B
	bl	printf

	pop	{lr}
	mov r0, #0 // return 0
	bx lr
