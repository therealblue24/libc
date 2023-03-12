#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main()
{
	int ret = 0;
	printf("Hello, World! %d %s\n", 5, "str");
	int *a = (int *)malloc(sizeof(int));
	printf("%p\n", a);
	a[0] = 5;
	printf("%d\n", a[0]);
	a = realloc(a, sizeof(int) * 3);
	a[1] = 4;
	a[2] = 3;
	printf("%d, %d, %d\n%p\n", a[0], a[1], a[2], a);
	int *b = (int *)malloc(sizeof(int));
	printf("%p\n", b);
	free(a);
	free(b);
	return ret;
}
