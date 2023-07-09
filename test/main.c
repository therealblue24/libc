#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

extern int pipe(int[2]);
extern int close(int);
extern long write(int, const char *, unsigned long);

#define fail(x) _fail(__func__, x)
#define ok(x) _ok(__func__)

int status = 0;
int fd[2];
int testnum_ok = 0;
int testnum_notok = 0;
int already_failed = 0;

static void _init_ptrv()
{
    pipe(fd);
}

static void _deinit_ptrv()
{
    close(fd[0]);
    close(fd[1]);
}

static int validate_ptr(const void *ptr, long bytes)
{
    if(ptr == NULL)
        return 0;
    int ret = 1;
    if(write(fd[1], ptr, bytes) < 0) {
        ret = 0;
    }
    if(ret) {
        printf("[validate_ptr] ptr %p is correct\n", ptr);
    } else {
        printf("[validate_ptr] ptr %p is incorrect\n", ptr);
    }
    return ret;
}

void _fail(const char *test, const char *msg)
{
    if(!already_failed)
        printf("%s: [%s]\n", test, msg);
    status = 1;
    if(!already_failed)
        testnum_notok++;
    if(!already_failed)
        testnum_ok--;
    already_failed = 1;
}

void _ok(const char *test)
{
    if(!status)
        printf("%s: [ok]\n", test);
    testnum_ok++;
    already_failed = 0;
}

static void malloc_test()
{
    int *a = malloc(4);
    if(!a) {
        fail("allocation failed");
    }
    if(!validate_ptr(a, 4)) {
        fail("not in address space");
    }
    if(a)
        *a = 2;
    if(a)
        free(a);
    ok();
}

static void align_alloc_test()
{
    int *a = aligned_alloc(4096, 4);
    if(!a) {
        fail("allocation failed");
    }
    if(!validate_ptr(a, 4)) {
        fail("not in address space");
    }
    if(a)
        *a = 2;
    long *b = aligned_alloc(4096, 8);
    if(!b) {
        fail("allocation failed");
    }
    if(!validate_ptr(b, 8)) {
        fail("not in address space");
    }
    if(b)
        *b = 4;
    if(a)
        free(a);
    if(b)
        free(b);
    ok();
}

static void valloc_test()
{
    int *a = valloc(4);
    if(!a) {
        fail("allocation failed");
    }
    if(!validate_ptr(a, 4)) {
        fail("not in address space");
    }
    if(a)
        *a = 2;
    if(a)
        free(a);
    ok();
}

int main()
{
    _init_ptrv();
    malloc_test();
    align_alloc_test();
    valloc_test();
    _deinit_ptrv();
    printf("\nTotal of %d tests, %d passed and %d failed.\n",
           testnum_ok + testnum_notok, testnum_ok, testnum_notok);
    printf("Test result: ");
    if(testnum_notok != 0) {
        printf("failed\n");
    } else {
        printf("passed\n");
    }
    return status;
}
