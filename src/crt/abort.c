#include <stdlib.h>

[[gnu::weak]] void abort(void)
{
    exit(3440);
}
