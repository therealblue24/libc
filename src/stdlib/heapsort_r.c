#include <stddef.h>
#include <stdlib.h>

/*
 * Swap two areas of size number of bytes.  Although qsort(3) permits random
 * blocks of memory to be sorted, sorting pointers is almost certainly the
 * common case (and, were it not, could easily be made so).  Regardless, it
 * isn't worth optimizing; the SWAP's get sped up by the cache, and pointer
 * arithmetic gets lost in the time required for comparison function calls.
 */
#define SWAP(a, b, count, size, tmp) \
	{                                \
		count = size;                \
		do {                         \
			tmp = *a;                \
			*a++ = *b;               \
			*b++ = tmp;              \
		} while(--count);            \
	}

/* Copy one block of size size to another. */
#define COPY(a, b, count, size, tmp1, tmp2) \
	{                                       \
		count = size;                       \
		tmp1 = a;                           \
		tmp2 = b;                           \
		do {                                \
			*tmp1++ = *tmp2++;              \
		} while(--count);                   \
	}

/*
 * Build the list into a heap, where a heap is defined such that for
 * the records K1 ... KN, Kj/2 >= Kj for 1 <= j/2 <= j <= N.
 *
 * There two cases.  If j == nmemb, select largest of Ki and Kj.  If
 * j < nmemb, select largest of Ki, Kj and Kj+1.
 */
#define CREATE(initval, nmemb, par_i, child_i, par, child, size, count, tmp) \
	{                                                                        \
		for(par_i = initval; (child_i = par_i * 2) <= nmemb;                 \
			par_i = child_i) {                                               \
			child = base + child_i * size;                                   \
			if(child_i < nmemb && compar(thunk, child, child + size) < 0) {  \
				child += size;                                               \
				++child_i;                                                   \
			}                                                                \
			par = base + par_i * size;                                       \
			if(compar(thunk, child, par) <= 0)                               \
				break;                                                       \
			SWAP(par, child, count, size, tmp);                              \
		}                                                                    \
	}

/*
 * Select the top of the heap and 'heapify'.  Since by far the most expensive
 * action is the call to the compar function, a considerable optimization
 * in the average case can be achieved due to the fact that k, the displaced
 * elememt, is ususally quite small, so it would be preferable to first
 * heapify, always maintaining the invariant that the larger child is copied
 * over its parent's record.
 *
 * Then, starting from the *bottom* of the heap, finding k's correct place,
 * again maintianing the invariant.  As a result of the invariant no element
 * is 'lost' when k is assigned its correct place in the heap.
 *
 * The time savings from this optimization are on the order of 15-20% for the
 * average case. See Knuth, Vol. 3, page 158, problem 18.
 *
 * XXX Don't break the #define SELECT line, below.  Reiser cpp gets upset.
 */
#define SELECT(par_i, child_i, nmemb, par, child, size, k, count, tmp1, tmp2) \
	{                                                                         \
		for(par_i = 1; (child_i = par_i * 2) <= nmemb; par_i = child_i) {     \
			child = base + child_i * size;                                    \
			if(child_i < nmemb && compar(thunk, child, child + size) < 0) {   \
				child += size;                                                \
				++child_i;                                                    \
			}                                                                 \
			par = base + par_i * size;                                        \
			COPY(par, child, count, size, tmp1, tmp2);                        \
		}                                                                     \
		for(;;) {                                                             \
			child_i = par_i;                                                  \
			par_i = child_i / 2;                                              \
			child = base + child_i * size;                                    \
			par = base + par_i * size;                                        \
			if(child_i == 1 || compar(thunk, k, par) < 0) {                   \
				COPY(child, k, count, size, tmp1, tmp2);                      \
				break;                                                        \
			}                                                                 \
			COPY(child, par, count, size, tmp1, tmp2);                        \
		}                                                                     \
	}

int heapsort_r(void *vbase, size_t nmemb, size_t size, void *thunk,
			   int (*compar)(void *, const void *, const void *))
{
	size_t cnt;
	size_t i;
	size_t j;
	size_t l;
	char tmp;
	char *tmp1;
	char *tmp2;
	char *base;
	char *k;
	char *p;
	char *t;

	if(nmemb <= 1) {
		return (0);
	}

	if(!size) {
		// errno = EINVAL;
		return (-1);
	}

	if((k = malloc(size)) == NULL) {
		return (-1);
	}

	base = (char *)vbase - size;

	for(l = nmemb / 2 + 1; --l;) {
		CREATE(l, nmemb, i, j, t, p, size, cnt, tmp);
	}

	while(nmemb > 1) {
		COPY(k, base + nmemb * size, cnt, size, tmp1, tmp2);
		COPY(base + nmemb * size, base + size, cnt, size, tmp1, tmp2);
		--nmemb;
		SELECT(i, j, nmemb, t, p, size, k, cnt, tmp1, tmp2);
	}

	free(k);
	return (0);
}

#undef SWAP
#undef COPY
#undef CREATE
#undef SELECT
