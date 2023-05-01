#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <setjmp.h>

#define fail(x) _fail(__func__, x)
#define ok(x) _ok(__func__)

int status = 0;

void _fail(const char *test, const char *msg)
{
    printf("%s: [%s]\n", test, msg);
    status = 1;
}

void _ok(const char *test)
{
    printf("%s: [ok]\n", test);
}

static void malloc_test()
{
    int *a = malloc(4);
    if(!a)
        fail("allocation failed");
    free(a);
    ok();
}

int main()
{
    malloc_test();
    return status;
}
