.text
.global main

main:
	push {lr}
loop:
	ldr r0, =stdin
	ldr r0, [r0]
	bl	fgetc
	cmp	r0, #-1 // EOF
	beq	end_loop
	
	cmp r0, #'0'
	blt	loop
	cmp r0, #'9'
	bgt	loop
	
	ldr r1, =stdout
	ldr r1, [r1]
	bl	fputc
	b	loop

end_loop:
	pop	{lr}
	bx	lr

