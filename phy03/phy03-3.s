.text
.global main

main:
    push {r4, r5, r6, lr}
	mov r0, #0
	mov r1, #4
	bl	realloc // reallocate 4 bytes
	mov	r4, r0	// write pointer to r4
	mov r5, #0	// use r5 as counter

read_loop:
    ldr r0, =stdin
    ldr r0, [r0]
    bl  fgetc
    cmp r0, #-1 // EOF
    beq write_loop
	
	str	r0, [r4, r5, lsl #2] // store r0 to r4+4*r5
	add	r5, r5, #1
	mov r0, r4	// move pointer to r0 for reallocation
	mov r1, r5
	add r1, r1, #1
	mov r6, #4	// for mul
	mul r1, r1, r6 // r1=4*(r5+1)
	bl	realloc
	mov r4, r0	// move pointer to r4
    b   read_loop

write_loop:
	sub r5, r5, #1
	cmp	r5,	#0
	blt	end
	ldr r0,	[r4, r5, lsl #2]
	ldr r1, =stdout
	ldr r1, [r1]
	bl	fputc
	b	write_loop

end:
	mov r0, r4
	bl	free
    pop {r4, r5, r6, lr}
    bx  lr

