#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <setjmp.h>
#include <cmocka.h>

static void null_test_success(void **state)
{
	(void)state;
}

static void malloc_test(void **state)
{
	(void)state;
	int *a = malloc(4);
	if(!a)
		fail();
	free(a);
}

int main()
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(null_test_success),
		cmocka_unit_test(malloc_test),
	};
	cmocka_run_group_tests(tests, NULL, NULL);
	exit(0);
	printf("How did you get here?\n");
	return 0;
}
