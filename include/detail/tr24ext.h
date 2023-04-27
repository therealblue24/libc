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
    uint8_t ptr[];
};

#define __tr24_fat_hdrget(p)                \
    ((struct __cctr24(fat_ptr_hdr) *)((p) - \
                                      (sizeof(struct __cctr24(fat_ptr_hdr)))))

inline size_t __tr24_fps() {
    return sizeof(__tr24_fat_ptr_hdr);
}

#define __tr24_fat_hdrgetvar(p)        \
    struct __cctr24(fat_ptr_hdr) *sh = \
        (void *)((p) - (sizeof(struct __cctr24(fat_ptr_hdr))));

#ifdef __cplusplus
}
#endif

#endif /* TR24EXT_H_ */
