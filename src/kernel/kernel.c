#include <stddef.h>
#include <stdint.h>

static inline void mmio_write(uint32_t reg, uint32_t data) {
	// Write 4 byte word to register
	*(volatile uint32_t*)reg = data;
}

static inline uint32_t mmio_read(uint32_t reg) {
	// Read 4 byte word from register
	return *(volatile uint32_t*)reg;
}

// loop <delay> times in a way that compiler wont optimize it away
static inline void delay(int32_t count) {
	asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n" : "=r"(count): [count]"0"(count) : "cc");
}

enum {
	// GPIO registers base address
	GPIO_BASE = 0x3F200000,

	GPPUD = (GPIO_BASE + 0x94),
	GPPUDCLK0 = (GPIO_BASE + 0x98),

	// Base address for UART
	UART0_BASE = 0x3F201000,

	UART0_DR 	 = (UART0_BASE + 0x00),
	UART0_RSRECR = (UART0_BASE + 0x04),
	UART0_FR 	 = (UART0_BASE + 0x18),
	UART0_ILPR 	 = (UART0_BASE + 0x20),
	UART0_IBRD 	 = (UART0_BASE + 0x24),
	UART0_FBRD 	 = (UART0_BASE + 0x28),
	UART0_LCRH 	 = (UART0_BASE + 0x2C),
	UART0_CR 	 = (UART0_BASE + 0x30),
	UART0_IFLS   = (UART0_BASE + 0x34),
    UART0_IMSC   = (UART0_BASE + 0x38),
    UART0_RIS    = (UART0_BASE + 0x3C),
    UART0_MIS    = (UART0_BASE + 0x40),
    UART0_ICR    = (UART0_BASE + 0x44),
    UART0_DMACR  = (UART0_BASE + 0x48),
    UART0_ITCR   = (UART0_BASE + 0x80),
    UART0_ITIP   = (UART0_BASE + 0x84),
    UART0_ITOP   = (UART0_BASE + 0x88),
    UART0_TDR    = (UART0_BASE + 0x8C),
};

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
