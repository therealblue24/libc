#include <stdio.h>

int puts(const char *str)
{
	int r = 0;
	for(const char *c = str; *c != 0; c++) {
		putchar((int)*c);
		r++;
	}
	if(r) {
		putchar('\n');
		r++;
	}
	return r ? r : EOF;
}
