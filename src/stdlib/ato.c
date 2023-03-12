#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

double atof(const char *ascii)
{
	return (strtod(ascii, NULL));
}

int atoi(const char *str)
{
	bool neg = false;
	int val = 0;

	switch(*str) {
	case '-':
		neg = true;
		[[fallthrough]];
	case '+':
		str++;
	default:
		break;
	}

	while(isdigit(*str)) {
		val = (10 * val) + (*str++ - '0');
	}

	return (neg ? -val : val);
}

long atol(const char *str)
{
	long val = 0;
	bool neg = false;

	while(isspace(*str)) {
		str++;
	}

	switch(*str) {
	case '-':
		neg = true;
		[[fallthrough]];
	case '+':
		str++;
	default:
		break;
	}

	while(isdigit(*str)) {
		val = (10 * val) + (*str++ - '0');
	}

	return neg ? -val : val;
}

long long atoll(const char *str)
{
	long long val = 0;
	bool neg = false;

	while(isspace(*str)) {
		str++;
	}

	switch(*str) {
	case '-':
		neg = true;
		[[fallthrough]];
	case '+':
		str++;
	default:;
	}

	while(isdigit(*str)) {
		val = (10 * val) + (*str++ - '0');
	}

	return neg ? -val : val;
}
