#include <stdlib.h>

div_t div(int x, int y)
{
	div_t r;
	r.quot = x / y;
	r.rem = x % y;
	if(x >= 0 && r.rem < 0) {
		r.quot++;
		r.rem -= y;
	}
	return (r);
}

ldiv_t ldiv(long x, long y)
{
	ldiv_t r;
	r.quot = x / y;
	r.rem = x % y;
	if(x >= 0 && r.rem < 0) {
		r.quot++;
		r.rem -= y;
	}
	return (r);
}

lldiv_t lldiv(long long x, long long y)
{
	lldiv_t r;
	r.quot = x / y;
	r.rem = x % y;
	if(x >= 0 && r.rem < 0) {
		r.quot++;
		r.rem -= y;
	}
	return (r);
}
