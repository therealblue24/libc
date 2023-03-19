#include <stdint.h>
#include <string.h>

[[gnu::weak]] void *memset(void *dest, int c, size_t n)
{
	unsigned char *s = dest;
	size_t k;

	if(!n) {
		return dest;
	}

	s[0] = s[n - 1] = (unsigned char)c;

	if(n <= 2) {
		return dest;
	}

	s[1] = s[n - 2] = (unsigned char)c;
	s[2] = s[n - 3] = (unsigned char)c;

	if(n <= 6) {
		return dest;
	}

	s[3] = s[n - 4] = (unsigned char)c;

	if(n <= 8) {
		return dest;
	}

	k = -(uintptr_t)s & 3;
	s += k;
	n -= k;
	n &= (unsigned long)-4;
	n /= 4;

	uint32_t *ws = (uint32_t *)(void *)s;
	uint32_t wc = c & 0xFF;
	wc |= ((wc << 8) | (wc << 16) | (wc << 24));

	for(; n; n--, ws++) {
		*ws = wc;
	}

	return dest;
}
