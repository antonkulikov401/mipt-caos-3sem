.text
.global R
.global A
.global B
.global C
.global D

.global calculate
calculate:
	ldr	r1, =A
	ldr r1, [r1]
	ldr r2, =B
	ldr r2, [r2]
	mul	r1, r1, r2
	mov r0, r1	// r0 = A*B
	
	ldr r1, =C
	ldr r1, [r1]
	ldr r2, =D
	ldr r2, [r2]
	mul r1, r1, r2
	add r0, r0, r1 // r0 = (A*B)+(C*D)
	
	ldr r1, =R
	str r0, [r1]

	bx lr

.data
R:
	.word

