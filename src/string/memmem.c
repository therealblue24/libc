#include <string.h>

void *memmem(const void *l, size_t l_len, const void *s, size_t s_len)
{
	const char *cur;
	const char *last;
	const char *cl = (const char *)l;
	const char *cs = (const char *)s;

	if(l_len == 0 || s_len == 0) {
		return NULL;
	}

	if(l_len < s_len) {
		return NULL;
	}

	if(s_len == 1) {
		return memchr(l, (int)*cs, l_len);
	}

	last = cl + l_len - s_len;

	for(cur = cl; cur <= last; cur++) {
		if(cur[0] == cs[0] && memcmp(cur, cs, s_len) == 0) {
			return (void *)(uintptr_t)cur;
		}
	}

	return NULL;
}
