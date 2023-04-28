#ifndef __APPLE__

#include <crt.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

extern int __bss_start__;
extern int __bss_end__;

[[gnu::weak]] extern void (*__preinit_array_start[])(void);
[[gnu::weak]] extern void (*__preinit_array_end[])(void);
[[gnu::weak]] extern void (*__init_array_start[])(void);
[[gnu::weak]] extern void (*__init_array_end[])(void);
[[gnu::weak]] extern void (*__fini_array_start[])(void);
[[gnu::weak]] extern void (*__fini_array_end[])(void);
extern int main();

[[gnu::weak]] int entry(void)
{
    CRTStartup();
    return main();
}

/* Can't use C23 attributes here as gnu::no_stack_protector doesn't exist. */
__attribute__((no_stack_protector)) void __libc_init_array(void)
{
    size_t count = (size_t)(__preinit_array_end - __preinit_array_start);
    for(size_t i = 0; i < count; i++) {
        __preinit_array_start[i]();
    }

    count = (size_t)(__init_array_end - __init_array_start);
    for(size_t i = 0; i < count; i++) {
        __init_array_start[i]();
    }
}

void __libc_fini_array(void)
{
    size_t count = (size_t)(__fini_array_end - __fini_array_start);
    for(size_t i = count; i > 0; i--) {
        __fini_array_start[i - 1]();
    }
}

__attribute__((no_stack_protector)) void CRTStartup(void)
{
    memset(&__bss_start__, 0,
           (uintptr_t)&__bss_end__ - (uintptr_t)&__bss_start__);

    __libc_init_array();

    /* TODO: handle relocs? */
}
#endif
