#include <alloc/malloc.h>
#include <memory.h>
#include <stdint.h>

#ifndef FREELIST_DECL_SPECIFIERS
#define FREELIST_DECL_SPECIFIERS static
#endif

#ifndef align_up
#define align_up(num, align) (((num) + ((align)-1)) & ~((align)-1))
#endif

#define alloca(size) __builtin_alloca(size)

typedef struct {
    ll_t node;
    size_t size;
    char *block;
} alloc_node_t;

#define ALLOC_HEADER_SZ offsetof(alloc_node_t, block)
#define MIN_ALLOC_SZ ALLOC_HEADER_SZ + 32
static void defrag_free_list(void);
void malloc_lock();
void malloc_unlock();
FREELIST_DECL_SPECIFIERS LIST_INIT(free_list);

#ifndef NULL
#define NULL ((void *)0)
#endif

void defrag_free_list(void)
{
    alloc_node_t *block = NULL;
    alloc_node_t *last_block = NULL;
    alloc_node_t *temp = NULL;

    list_for_each_entry_safe(block, temp, &free_list, node) {
        if(last_block) {
            if((((uintptr_t)&last_block->block) + last_block->size) ==
               (uintptr_t)block) {
                last_block->size += ALLOC_HEADER_SZ + block->size;
                list_del(&block->node);
                continue;
            }
        }
        last_block = block;
    }
}

[[gnu::weak]] void malloc_init(void)
{
}

[[gnu::weak]] void malloc_lock()
{
    // Intentional no-op
}

[[gnu::weak]] void malloc_unlock()
{
    // Intentional no-op
}

extern void *sbrk(intptr_t increment);
void *ptr_loc = 0;

void *malloc(size_t size)
{
    if(ptr_loc == 0) {
        ptr_loc = sbrk(0);
        malloc_addblock(ptr_loc, (1024 * 1024));
    }
    void *ptr = NULL;
    alloc_node_t *found_block = NULL;
    if(size > 0) {
        size = align_up(size, sizeof(void *));
        malloc_lock();
        list_for_each_entry(found_block, &free_list, node) {
            if(found_block->size >= size) {
                ptr = &found_block->block;
                break;
            }
        }
        if(ptr) {
            if((found_block->size - size) >= MIN_ALLOC_SZ) {
                alloc_node_t *new_block =
                    (alloc_node_t *)((uintptr_t)(&found_block->block) + size);
                new_block->size = found_block->size - size - ALLOC_HEADER_SZ;
                found_block->size = size;
                list_insert(&new_block->node, &found_block->node,
                            found_block->node.next);
            }
            list_del(&found_block->node);
        }
        malloc_unlock();
    }
    if(ptr == NULL) {
        ptr_loc += (1024 * 1024);
        malloc_addblock(ptr_loc, (1024 * 1024));
        return malloc(size);
    }
    return ptr;
}

void free(void *ptr)
{
    if(ptr) {
        alloc_node_t *current_block = container_of(ptr, alloc_node_t, block);
        alloc_node_t *free_block = NULL;
        malloc_lock();
        list_for_each_entry(free_block, &free_list, node) {
            if(free_block > current_block) {
                list_insert(&current_block->node, free_block->node.prev,
                            &free_block->node);
                goto blockadded;
            }
        }
        list_add_tail(&current_block->node, &free_list);
        ptr = NULL;
blockadded:
        defrag_free_list();
        malloc_unlock();
        ptr = NULL;
    }
}

void malloc_addblock(void *addr, size_t size)
{
    alloc_node_t *new_memory_block =
        (void *)align_up((uintptr_t)addr, sizeof(void *));
    new_memory_block->size =
        (uintptr_t)addr + size - (uintptr_t)new_memory_block - ALLOC_HEADER_SZ;
    malloc_lock();
    list_add(&new_memory_block->node, &free_list);
    malloc_unlock();
}

#undef list_entry
#undef list_first_entry
#undef list_for_each
#undef list_for_each_safe
#undef list_for_each_entry
#undef list_for_each_entry_safe
#undef ll_head_INIT
#undef LIST_INIT

#undef list_insert
#undef list_add
#undef list_add_tail
#undef list_join_nodes
#undef list_del
