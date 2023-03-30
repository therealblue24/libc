#ifndef MALLOC_H_
#define MALLOC_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdlib.h>
#include <alloc/ll.h>

static void malloc_addblock(void *addr, size_t size);
void malloc_init(void);

#ifdef __cplusplus
}
#endif

#endif /* MALLOC_H_ */
