#ifndef TR24EXT_H_
#define TR24EXT_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef INTERNAL_H_
#include <detail/internal.h>
#endif /* INTERNAL_H_ */

#include <stdint.h>
#include <stdio.h>

typedef void *fat_ptr_t;

struct __tr24_fat_ptr_hdr {
    size_t alloced;
    uint8_t flags;
    void *ptr;
};

#define __tr24_fat_hdrget(p)                \
    ((struct __cctr24(fat_ptr_hdr) *)((p) - \
                                      (sizeof(struct __cctr24(fat_ptr_hdr)))))

inline size_t __tr24_fps()
{
    return sizeof(struct __tr24_fat_ptr_hdr);
}

#define __tr24_fat_hdrgetvar(p)        \
    struct __cctr24(fat_ptr_hdr) *sh = \
        (void *)((p) - (sizeof(struct __cctr24(fat_ptr_hdr))));

/* extensions: macros */

#define sanitize(x, y, z)                                                                                       \
    z = 0;                                                                                                      \
    if(!x) {                                                                                                    \
        printf(                                                                                                 \
            "null: nonnull function \"%s\" in file \"%s\" at line %u had an unsanitized pointer (aka null).\n", \
            __func__, __FILE__, __LINE__);                                                                      \
        if(y != 0)                                                                                              \
            x = (void *)malloc(y);                                                                              \
        z = 1;                                                                                                  \
    }

#define hard_sanitize(x)   \
    int local = 0;         \
    sanitize(x, 0, local); \
    abort();

#define _NONNULL [[gnu::nonnull]]
#define _NORETURN [[noreturn]]

#ifdef __cplusplus
}
#endif

#endif /* TR24EXT_H_ */
