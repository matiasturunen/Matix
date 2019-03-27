#include <common/stdio.h>
#include <kernel/uart.h>
#include <kernel/gpu.h>

char getc() {
	return uart_getc();
}

void putc(char c) {
	//uart_putc(c);
	gpu_putc(c); // Use gpu
}

void gets(char * buf, int buflen) {
	int i; 
	char c;

	// Leave spot for null cahr in buffer
	for (i = 0; (c = getc()) != '\r' && buflen > 1; i++, buflen--) {
		putc(c); // Echo out written characters
		buf[i] = c;
	}

	putc('\n');
	if (c == '\n') {
		buf[i] = '\0';
	} else {
		buf[buflen-1] = '\0';
	}
}

void puts(const char* str) {
	int i;
	for (i = 0; str[i] != '\0'; i++) {
		putc(str[i]);
	}
}
