#include <printf.h>
#include <stdio.h>

extern long write(int, const char *, unsigned long);

void putchar_(char ch)
{
	(void)write(1, &ch, 1);
}

int putchar(int c)
{
	putchar_((char)c);
	return c;
}

char putcchar(char c)
{
	putchar((int)(c % 256));
	return c;
}
