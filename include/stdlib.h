#ifndef STDLIB_H_
#define STDLIB_H_

#include <stddef.h>
#include <stdint.h>
#include <wctype.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int quot;
	int rem;
} div_t;

typedef struct {
	long quot;
	long rem;
} ldiv_t;

typedef struct {
	long long quot;
	long long rem;
} lldiv_t;

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define RAND_MAX (0x7fffffff)
#define MB_CUR_MAX sizeof(wchat_t)

void abort(void);
int atexit(void (*)(void));
void exit(int);

void _Exit(int);
int at_quick_exit(void (*)(void));
void quick_exit(int);
int cxa_atexit(void (*)(void *), void *, void *);

/* todo */
char *getenv(const char *);
int system(const char *);
int mblen(const char *, size_t);
int mbtowc(wchar_t *__restrict, const char *__restrict, size_t);
int wctomb(char *, wchar_t);
size_t mbstowcs(wchar_t *__restrict, const char *__restrict, size_t);
size_t wcstombs(char *__restrict, const wchar_t *__restrict, size_t);

/* end todo */

int atoi(const char *str);
long atol(const char *str);
long long atoll(const char *str);
double atof(const char *str);
float strtof(const char *__restrict str, char **__restrict str_end);
double strtod(const char *__restrict str, char **__restrict str_end);
long strtol(const char *__restrict str, char **__restrict str_end, int base);
unsigned long strtoul(const char *__restrict str, char **__restrict str_end,
					  int base);
long long strtoll(const char *__restrict str, char **__restrict str_end,
				  int base);
unsigned long long strtoull(const char *__restrict str,
							char **__restrict str_end, int base);
/* todo */
long double strtold(const char *__restrict, char **__restrict);
/* end todo */

int abs(int n);
long labs(long n);
long long llabs(long long n);
div_t div(int x, int y);
ldiv_t ldiv(long x, long y);
lldiv_t lldiv(long long x, long long y);
int rand_r(unsigned int *ctx);
int rand(void);
void srand(unsigned seed);
int heapsort(void *vbase, size_t nmemb, size_t size,
			 int (*compar)(const void *, const void *));
int heapsort_r(void *vbase, size_t nmemb, size_t size, void *thunk,
			   int (*compar)(void *, const void *, const void *));
void *bsearch(const void *key, const void *ptr, size_t count, size_t size,
			  int (*comp)(const void *, const void *));
void qsort_r(void *a, size_t n, size_t es, void *thunk,
			 int (*cmp)(void *, const void *, const void *));
void qsort(void *a, size_t n, size_t es,
		   int (*compar)(const void *, const void *));
void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t num, size_t size);
void *realloc(void *ptr, size_t size);
void *reallocf(void *ptr, size_t size);

#ifdef _POSIX_MEMALIGN_VISIBLE

int posix_memalign(void **memptr, size_t alignment, size_t size);
#endif /* _POSIX_MEMALIGN_VISIBLE */

#if(defined(__ISO_C_VISIBLE) && __ISO_C_VISIBLE >= 2011) ||  \
	(defined(__STDC_VERSION) && __STDC_VERSION >= 20112L) || \
	(defined(__cplusplus) && __cplusplus >= 201103L)

void *aligned_alloc(size_t align, size_t size);

#endif /* jumble */

/* extensions */

#define sanitize(x, y, z)                                                                                                                                           \
	z = 0;                                                                                                                                                          \
	if(!x) {                                                                                                                                                        \
		printf(                                                                                                                                                     \
			"NULL! nonnull function \"%s\" in file \"%s\" at line %u had an unsanitized pointer (aka null). program is going to continue but this is a warning.\n", \
			__func__, __FILE__, __LINE__);                                                                                                                          \
		x = (void *)malloc(y);                                                                                                                                      \
		z = 1;                                                                                                                                                      \
	}

#define hard_sanitize(x, y, z) \
	sanitize(x, y, z);         \
	abort();

#ifdef __cplusplus
}
#endif

#endif /* STDLIB_H_ */
#include <alloc/malloc.h>
