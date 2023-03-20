#include <limits.h>
#include <string.h>

/* Yes, this doesn't use the classical 'find 0 return length'-only method,
 * it uses optizations to make it ~5x faster.
 */

#if LONG_BIT == 32
static const unsigned long mask01 = 0x01010101;
static const unsigned long mask80 = 0x80808080;
#elif LONG_BIT == 64
static const unsigned long mask01 = 0x0101010101010101;
static const unsigned long mask80 = 0x8080808080808080;
#else
#error Unsupported word size
#endif

#define LONGPTR_MASK (sizeof(long) - 1)

#define testbyte(x)                                     \
    do {                                                \
        if(p[x] == '\0')                                \
            return ((uintptr_t)p - (uintptr_t)str + x); \
    } while(0)

size_t strlen(const char *str)
{
    const char *p;
    const unsigned long *lp;

    for(p = str; (uintptr_t)p & LONGPTR_MASK; p++) {
        if(*p == '\0') {
            return ((uintptr_t)p - (uintptr_t)str);
        }
    }

    for(lp = (const unsigned long *)(const void *)p;; lp++) {
        if((*lp - mask01) & mask80) {
            p = (const char *)(lp);
            testbyte(0);
            testbyte(1);
            testbyte(2);
            testbyte(3);
#if(LONG_BIT >= 64)
            testbyte(4);
            testbyte(5);
            testbyte(6);
            testbyte(7);
#endif
        }
    }

    /* NOTREACHED */
    /* return (0); */
}

#undef testbyte
