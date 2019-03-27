#include <common/stdlib.h>

void memcpy (void * dest, void * src, int bytes) {
	char * d = dest;
	const char * s = src;
	while (bytes--) {
		*d++ = *s++;
	}
}

void bzero (void * dest, int bytes) {
	char * d = dest;
	while (bytes--) {
		*d++ = 0;
	}
}

char * itoa(int i) {
	static char intbuf[12];
	int j = 0, isneg = 0;

	if (i == 0) {
		intbuf[0] = '0';
		intbuf[1] = '\0';
		return intbuf;
	}

	if (i < 0) {
		isneg = 1;
		j = -i;
	}

	while (i != 0) {
		intbuf[j++] = '0' + (i % 10);
		i /= 10;
	}

	if (isneg) {
		intbuf[j++] = '-';
	}

	intbuf[j] = '\0';
	j--;
	i = 0;
	while (i < j) {
		isneg = intbuf[i];
		intbuf[i] = intbuf[j];
		intbuf[j] = isneg;
		i++;
		j--;
	}
	return intbuf;
}

uint32_t div(uint32_t dividend, uint32_t divisor) {
	return dividend / divisor;
}

divmod_t divmod(uint32_t dividend, uint32_t divisor) {
	divmod_t res;
	res.div = dividend / divisor;
	res.mod = dividend % divisor;

	return res;
}

int atoi(char* num) {
	int res = 0;
	int power = 0;
	int digit;
	int i;
	char * start = num;

	// Find the end
	while (*num >= '0' && *num <= '9') {
		num++;
	}

	num--;

	while (num != start) {
		digit = *num - '0';
		for (i=0; i < power; i++) {
			digit *= 10;
		}
		res += digit;
		power++;
		num--;
	}

	return res;
}