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
	free(a);
	return ret;
}
