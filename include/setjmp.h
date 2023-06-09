#ifndef __SETJMP_H_
#define __SETJMP_H_

#include <_types/_setjmp.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __jmp_buf_tag {
    __jmp_buf __jb;
    unsigned long __fl;
    unsigned long __ss[128 / sizeof(long)];
} jmp_buf[1];

typedef jmp_buf sigjmp_buf;
/* todo */
int sigsetjmp(sigjmp_buf, int);
void siglongjmp(sigjmp_buf, int);
int _setjmp(jmp_buf);
int setjmp(jmp_buf);
void _longjmp(jmp_buf, int);
void longjmp(jmp_buf, int);

#ifdef __cplusplus
}
#endif

#endif /* __SETJMP_H_ */
