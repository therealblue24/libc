#include <string.h>

typedef int word;

#define wsize sizeof(word)
#define wmask (wsize - 1)

void *memcpy(void *dst0, const void *src0, size_t length)
{
	char *dst = dst0;
	const char *src = src0;
	size_t t;

	if(length == 0 || dst == src) {
		goto done;
	}
#define TLOOP(s) \
	if(t)        \
	TLOOP1(s)
#define TLOOP1(s) \
	do {          \
		s;        \
	} while(--t)
	if((uintptr_t)dst < (uintptr_t)src) {
		t = (uintptr_t)src;
		if((t | (uintptr_t)dst) & wmask) {
			if((t ^ (uintptr_t)dst) & wmask || length < wsize) {
				t = length;
			} else {
				t = wsize - (t & wmask);
			}
			length -= t;
			TLOOP1(*dst++ = *src++);
		}
		t = length / wsize;
		TLOOP(*(word *)(void *)dst = *(const word *)(const void *)src;
			  src += wsize; dst += wsize);
		t = length & wmask;
		TLOOP(*dst++ = *src++);
	} else {
		src += length;
		dst += length;
		t = (uintptr_t)src;

		if((t | (uintptr_t)dst) & wmask) {
			if((t ^ (uintptr_t)dst) & wmask || length <= wsize) {
				t = length;
			} else {
				t &= wmask;
			}

			length -= t;
			TLOOP1(*--dst = *--src);
		}

		t = length / wsize;
		TLOOP(src -= wsize; dst -= wsize;
			  *(word *)(void *)dst = *(const word *)(const void *)src);
		t = length & wmask;
		TLOOP(*--dst = *--src);
	}
done:
	return (dst0);
}
