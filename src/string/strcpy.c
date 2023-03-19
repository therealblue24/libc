#include <string.h>

/* Yes, strcpy is a wrapped memcpy. */
char *strcpy(char *__restrict dst, const char *__restrict src)
{
	const size_t length = strlen(src);
	memcpy(dst, src, length + 1);
	return dst;
}
