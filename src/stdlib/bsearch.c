#include <stddef.h>
#include <stdlib.h>

void *bsearch(const void *key, const void *ptr, size_t count, size_t size,
              int (*comp)(const void *, const void *))
{
    const char *base = ptr;
    size_t lim;
    int cmp;
    const void *p;

    for(lim = count; lim != 0; lim >>= 1) {
        p = base + (lim >> 1) * size;
        cmp = (*comp)(key, p);
        if(cmp == 0) {
            return ((void *)(uintptr_t)p);
        }
        if(cmp > 0) {
            base = (const char *)p + size;
            lim--;
        }
    }
    return (NULL);
}
