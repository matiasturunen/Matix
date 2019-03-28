#include <stddef.h>
#include <stdint.h>
#include <kernel/uart.h>
#include <kernel/mem.h>
#include <common/stdio.h>
#include <kernel/gpu.h>
#include <common/stdlib.h>

#define BUFFER_SIZE 256

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
	
	char buf[BUFFER_SIZE];

	// Declare as unused
	(void) r0;
	(void) r1;
	(void) atags;
	
	uart_init(); // init UART

	// Init memory
	uart_puts("Initializing memory.\r\n");
	mem_init((atag_t *)atags);
	uart_puts("Memory ready.\r\n");

	uart_puts("Initializing GPU.\r\n");
	// init gpu
	gpu_init();
	uart_puts("GPU ready.\r\n");

	
	puts("Hello, kernel world!\r\n"); // Print HelloWorld

	// Print any typed character
	int i = 0;
	while(1) {
		//gets(buf, BUFFER_SIZE);
		//puts(buf);
		i++;
		puts(itoa(i));
		putc('\n');
	}
}
