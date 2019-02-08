.section ".text.boot"

.global _start

_start:
	mrc p15, #0, r1, c0, c0, #5 // shut down three of four cores
	and r1, r1, #3				// by sending them to halt
	cmp r1, #0
	bne halt

	mov sp, #0x8000				// Tell the computer that our c stack starts at this address

	ldr r4, =__bss_start		// load start address of bss section into register
	ldr r9, =__bss_end			// load end address of bss section into register
	
	mov r5, #0					// this code zeros out the bss section
	mov r6, #0					// first it loads 0 to four registers
	mov r7, #0
	mov r8, #0
	b 	2f

1:
	stmia r4!, {r5-r8}			// stm operator stores the second operand into the address contained in the first
								// ia means 'increment after' or increment the address in r4 to the address after the last address written by the instrucion
								// ! means to store that address back o r4 instead of throwing it out
								// The {r5-r8} operand means that stm should store the values in registers r5,r6,r7,r8 (16 bytes) to r4.
								// So in conclusions this instuction stores 16 bytes of zeros into address in r4, and then increment address by 16 bytes
								// This loops until r4 is greater than or equal to r9 and the whole bss section is zeroed out

2: 
	cmp r4, r9					// compare r4 and r9 registers. Check if r4 is less than r9 and if it is then execute stmia command
	blo 1b

	ldr r3, =kernel_main		// load c function called kernel_main into register
	blx r3						// and jump to that location

halt:
	wfe							// when c function returns jump to halt and loop forever
	b halt
