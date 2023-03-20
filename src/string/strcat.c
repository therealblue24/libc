#include <string.h>

char *strcat(char *__restrict dst, const char *__restrict src)
{
    const size_t dstlen = strlen(dst);
    const size_t srclen = strlen(src);
    memcpy(dst + dstlen, src, srclen + 1);
    return dst;
}
