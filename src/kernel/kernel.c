#include <stddef.h>
#include <stdint.h>
#include <kernel/uart.h>

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
	// Declare as unused
	(void) r0;
	(void) r1;
	(void) atags;

	uart_init(); // init UART
	uart_puts("Hello, kernel world!\r\n"); // Print HelloWorld

	// Print any typed character
	while(1) {
		uart_putc(uart_getc());
		uart_putc('\n');
	}
}
