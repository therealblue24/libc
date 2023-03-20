#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MUL_NO_OVERFLOW (1UL << (sizeof(size_t) * 4))

void *calloc(size_t num, size_t size)
{
    if((num >= MUL_NO_OVERFLOW || size >= MUL_NO_OVERFLOW) && num > 0 &&
       SIZE_MAX / num < size) {
        return NULL;
    }
    size_t total_size = num * size;
    void *ptr = malloc(total_size);
    if(ptr) {
        memset(ptr, 0, total_size);
    }
    return ptr;
}
