#ifndef __CRT_H_
#define __CRT_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CRT_HAS_INITFINI_ARRAY

void CRTStartup(void);
void __libc_init_array(void);
void __libc_fini_array(void);

int entry(void);

[[noreturn]] void __stack_chk_fail(void);

#ifndef DISABLE_STACK_CHK_GUARD_RUNTIME_CONFIG
uintptr_t __stack_chk_guard_init(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __CRT_H_ */
