	.text
	.global _start
	.syntax unified

_start:
	mov r7, 4
	mov r0, 1
	ldr r1, =msg
	mov r2, 13
	swi 0

end:
	mov r7, 1
	swi 0

msg:
	.ascii "Hello World\n"
