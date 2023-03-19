#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

[[gnu::weak]] void __assert_fail(const char *expr, const char *file,
								 unsigned int line, const char *function)
{
	printf("%s:%u: assertion in function %s failed expression %s\n", file, line,
		   function, expr);
	abort();
}
