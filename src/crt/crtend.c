#include <stdint.h>

#ifndef __APPLE__
const int32_t __EH_FRAME_LIST_END__[]
    __attribute__((section(".eh_frame"), aligned(sizeof(void *)),
                   visibility("hidden"), used)) = { 0 };
#endif

#ifndef CRT_HAS_INITFINI_ARRAY
typedef void (*fp)(void);
#ifndef __APPLE__
fp __CTOR_LIST_END__[]
    __attribute__((section(".ctors"), visibility("hidden"), used)) = { 0 };
fp __DTOR_LIST_END__[]
    __attribute__((section(".dtors"), visibility("hidden"), used)) = { 0 };
#endif
#endif
