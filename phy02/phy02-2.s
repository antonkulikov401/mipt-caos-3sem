.text
.global solve

solve:
	push {r4-r7}
	mov r4, #0 // current x
	mov r7, #254 // result

loop:
	cmp r4, #254
	bge done

	mov r5, r0
	mul r5, r5, r4
	mul r5, r5, r4
	mul r5, r5, r4 // r5 = A*x^3
	mov r6, r1
	mul r6, r6, r4
	mul r6, r6, r4 // r6 = B*x^2
	add r5, r5, r6 // r5 = A*x^3+B*x^2
	mov r6, r2
	mul r6, r4 // r6 = C*x
	add r5, r5, r6 // r5 = A*x^3+B*x^2+C*x
	add r5, r5, r3 // r5 = A*x^3+B*x^2+C*x+D
	
	cmp r5, #0
	beq refresh_minimum
next_iter:
	add r4, r4, #1
	b loop

done:
	mov r0, r7
	pop {r4-r7}
	bx lr

refresh_minimum:
	cmp r4, r7
	bge next_iter
	mov r7, r4
	b next_iter
