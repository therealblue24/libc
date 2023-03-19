#include <string.h>

[[gnu::weak]] int memcmp(const void *p1, const void *p2, size_t n)
{
	size_t i;

	if(p1 == p2) {
		return 0;
	}

	if(p1 == NULL) {
		return 1;
	}

	if(p2 == NULL) {
		return -1;
	}

	for(i = 0; (i < n) && (*(const uint8_t *)p1 == *(const uint8_t *)p2);
		i++, p1 = 1 + (const uint8_t *)p1, p2 = 1 + (const uint8_t *)p2) {
	}

	return (i == n) ? 0 : (*(const uint8_t *)p1 - *(const uint8_t *)p2);
}
