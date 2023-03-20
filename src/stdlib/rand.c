#include <stdlib.h>

static unsigned long next = 1;
static int do_rand(unsigned long *ctx)
{
#ifdef USE_WEAK_SEEDING
    return ((*ctx = *ctx * 1103515245 + 12345) % ((unsigned long)RAND_MAX + 1));
#else /* !USE_WEAK_SEEDING */
    long hi, lo, x;
    if(*ctx == 0) {
        *ctx = 123459876UL;
    }
    hi = (long)(*ctx / 127773L);
    lo = (long)(*ctx % 127773L);
    x = 16807 * lo - 2836 * hi;
    if(x < 0) {
        x += 0x7fffffff;
    }
    return (int)((*ctx = (unsigned long)x) % ((unsigned long)RAND_MAX + 1));
#endif /* USE_WEAK_SEEDING */
}

[[gnu::weak]] int rand_r(unsigned int *ctx)
{
    unsigned long val = (unsigned long)*ctx;
    int r = do_rand(&val);
    *ctx = (unsigned int)val;
    return (r);
}

[[gnu::weak]] int rand()
{
    return (do_rand(&next));
}

[[gnu::weak]] void srand(unsigned seed)
{
    next = seed;
}
