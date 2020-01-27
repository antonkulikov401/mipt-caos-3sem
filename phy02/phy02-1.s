.text
.global summ

summ:
	add r1, r2, r1, lsl #2
loop:
	cmp r2, r1
	bge done
	ldr r3, [r2]
	add r0, r0, r3
	add r2, r2, #4
	b loop
done:
	bx lr

