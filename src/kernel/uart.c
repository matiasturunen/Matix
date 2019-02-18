#include <stddef.h>
#include <stdint.h>
#include <kernel/uart.h>
#include <common/stdlib.h>

void mmio_write(uint32_t reg, uint32_t data) {
	// Write 4 byte word to register
	*(volatile uint32_t*)reg = data;
}

uint32_t mmio_read(uint32_t reg) {
	// Read 4 byte word from register
	return *(volatile uint32_t*)reg;
}

// loop <delay> times in a way that compiler wont optimize it away
void delay(int32_t count) {
	asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n" : "=r"(count): [count]"0"(count) : "cc");
}

void uart_init() {
	mmio_write(UART0_CR, 0x00000000); // Disable all UART hardware

	// Mark some GPIO pins disabled
	mmio_write(GPPUD, 0x00000000); // Mark that some pins should be disabled
	delay(150); // delay some time to give hardware some time to respond
	mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15)); // Disable pins 14 and 15
	delay(150); // delay some time to give hardware some time to respond
	mmio_write(GPPUDCLK0, 0x00000000); // Make pin disabling happen

	// Clear all pending interrupts from uart hardware
	mmio_write(UART0_ICR, 0x7FF);

	// Set baud rate
	mmio_write(UART0_IBRD, 1);
	mmio_write(UART0_FBRD, 40);

	// Write bits 4,5,6 to line contrl register. 4 means that UART harware will hold data in 8 item deep fifo, instead of 1 deep register
	// 5 and 6 mean that data sent or recieved will have 8bit long words
	mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6)); 

	// Disable all interrupts from UART by writing one of the relevant bits to interrrupt mask set clear register
	mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

	// Write bits 0,8,9 to control register. 0 enables UART hardware, 8 enables the ability to recieve data and 9 enables ability to transmit data
	mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

void uart_putc(unsigned char c) {
	while (mmio_read(UART0_FR) & (1 << 5)) {}
	mmio_write(UART0_DR, c);
}

unsigned char uart_getc() {
	while (mmio_read(UART0_FR) & (1 << 4)) {}
	return mmio_read(UART0_DR);
}

void uart_puts(const char* str) {
	for (size_t i=0; str[i] != '\0'; i++) {
		uart_putc((unsigned char)str[i]);
	}
}

