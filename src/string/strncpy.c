#include <string.h>

char *strncpy(char *__restrict dst, const char *__restrict src, size_t maxlen)
{
    const size_t srclen = strnlen(src, maxlen);
    if(srclen < maxlen) {
        memcpy(dst, src, srclen);
        memset(dst + srclen, 0, maxlen - srclen);
    } else {
        memcpy(dst, src, maxlen);
    }
    return dst;
}
