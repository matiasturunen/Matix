#include <stdint.h>
#include <kernel/gpu.h>
#include <kernel/framebuffer.h>
#include <common/stdlib.h>

#include <kernel/uart.h>

void write_pixel(uint32_t x, uint32_t y, const pixel_t * pix) {
	uint8_t * location = fbinfo.buf + y*fbinfo.pitch + x*BYTES_PER_PIXEL;
	memcpy(location, pix, BYTES_PER_PIXEL);
}

void gpu_putc(char c) {
	static const pixel_t WHITE = {0xdf, 0xdf, 0xdf}; // was 0xff, 0xff, 0xff
	static const pixel_t BLACK = {0x00, 0x00, 0x00};

	uint8_t w,h;
	uint8_t mask;
	const uint8_t * bmp = font(c);
	uint32_t i, num_rows = fbinfo.height/CHAR_HEIGHT;

	// shift everythig up 1 row
	if (fbinfo.chars_y >= num_rows) {
		// Copy a whole character row into the one above it
		for (i = 0; i < num_rows-1; i++) {
			memcpy(fbinfo.buf + fbinfo.pitch*i*CHAR_HEIGHT, fbinfo.buf + fbinfo.pitch*i*CHAR_HEIGHT, fbinfo.pitch * CHAR_HEIGHT);
		}
		// Zero out last row
		bzero(fbinfo.buf + fbinfo.pitch*i*CHAR_HEIGHT, fbinfo.pitch * CHAR_HEIGHT);
		fbinfo.chars_y--;
	}

	if (c == '\n') {
		fbinfo.chars_x = 0;
		fbinfo.chars_y++;
		return;
	}

	for (w = 0; w < CHAR_WIDTH; w++) {
		for (h = 0; h < CHAR_HEIGHT; h++) {
			mask = 1 << (w);
			if (bmp[h] & mask) {
				write_pixel(fbinfo.chars_x*CHAR_WIDTH + w, fbinfo.chars_y*CHAR_HEIGHT + h, &WHITE);
			} else {
				write_pixel(fbinfo.chars_x*CHAR_WIDTH + w, fbinfo.chars_y*CHAR_HEIGHT + h, &BLACK);
			}
		}
	}

	fbinfo.chars_x++;
	if (fbinfo.chars_x > fbinfo.chars_width) {
		fbinfo.chars_x = 0;
		fbinfo.chars_y++;
	}
	uart_puts("GPU_PUTC\r\n");
}

void gpu_init(void) {
	static const pixel_t BLACK = {0x00, 0x00, 0xdd};
	// Because framebuffer init does not always work, do it in a loop
	while(framebuffer_init());

	// Clear screen
	for (uint32_t j = 0; j < fbinfo.height; j++) {
		for (uint32_t i = 0; i < fbinfo.width; i++) {
			write_pixel(i,j,&BLACK);
			/*
uart_puts("j: ");
uart_puts(itoa(j));
uart_puts(" i: ");
uart_puts(itoa(i));
uart_puts("\r\n");
*/
		}
	}
}
