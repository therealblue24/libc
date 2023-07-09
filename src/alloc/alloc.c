#include <alloc/malloc.h>
#include <memory.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

extern int brk(void *);
extern void *sbrk(int);
extern int getpagesize();

#define HEADER_SIZE 8

#define MIN_ALLOC_LOG2 4
#define MIN_ALLOC ((size_t)1 << MIN_ALLOC_LOG2)

#define MAX_ALLOC_LOG2 31
#define MAX_ALLOC ((size_t)1 << MAX_ALLOC_LOG2)

#define BUCKET_COUNT (MAX_ALLOC_LOG2 - MIN_ALLOC_LOG2 + 1)

typedef struct list_t {
    struct list_t *prev, *next;
} list_t;

static list_t buckets[BUCKET_COUNT];

static size_t bucket_limit;

static uint8_t node_is_split[(1 << (BUCKET_COUNT - 1)) / 8];

static uint8_t *base_ptr;

static uint8_t *max_ptr;

static int update_max_ptr(uint8_t *new_value)
{
    if(new_value > max_ptr) {
        brk(new_value);
        max_ptr = new_value;
    }
    return 1;
}

static void list_init(list_t *list)
{
    list->prev = list;
    list->next = list;
}

static void list_push(list_t *list, list_t *entry)
{
    list_t *prev = list->prev;
    entry->prev = prev;
    entry->next = list;
    prev->next = entry;
    list->prev = entry;
}

static void list_remove(list_t *entry)
{
    list_t *prev = entry->prev;
    list_t *next = entry->next;
    prev->next = next;
    next->prev = prev;
}

static list_t *list_pop(list_t *list)
{
    list_t *back = list->prev;
    if(back == list)
        return NULL;
    list_remove(back);
    return back;
}

static uint8_t *ptr_for_node(size_t index, size_t bucket)
{
    return base_ptr +
           ((index - (1 << bucket) + 1) << (MAX_ALLOC_LOG2 - bucket));
}

static size_t node_for_ptr(uint8_t *ptr, size_t bucket)
{
    return ((ptr - base_ptr) >> (MAX_ALLOC_LOG2 - bucket)) + (1 << bucket) - 1;
}
static int parent_is_split(size_t index)
{
    index = (index - 1) / 2;
    return (node_is_split[index / 8] >> (index % 8)) & 1;
}

static void flip_parent_is_split(size_t index)
{
    index = (index - 1) / 2;
    node_is_split[index / 8] ^= 1 << (index % 8);
}

static size_t bucket_for_request(size_t request)
{
    size_t bucket = BUCKET_COUNT - 1;
    size_t size = MIN_ALLOC;

    while(size < request) {
        bucket--;
        size *= 2;
    }

    return bucket;
}

static int lower_bucket_limit(size_t bucket)
{
    while(bucket < bucket_limit) {
        size_t root = node_for_ptr(base_ptr, bucket_limit);
        uint8_t *right_child;

        if(!parent_is_split(root)) {
            list_remove((list_t *)base_ptr);
            list_init(&buckets[--bucket_limit]);
            list_push(&buckets[bucket_limit], (list_t *)base_ptr);
            continue;
        }

        right_child = ptr_for_node(root + 1, bucket_limit);
        if(!update_max_ptr(right_child + sizeof(list_t))) {
            return 0;
        }
        list_push(&buckets[bucket_limit], (list_t *)right_child);
        list_init(&buckets[--bucket_limit]);

        root = (root - 1) / 2;
        if(root != 0) {
            flip_parent_is_split(root);
        }
    }

    return 1;
}

static void *_malloc(size_t size)
{
    size_t original_bucket, bucket;

    if(size + HEADER_SIZE > MAX_ALLOC) {
        return NULL;
    }

    if(base_ptr == NULL) {
        base_ptr = max_ptr = (uint8_t *)sbrk(0);
        bucket_limit = BUCKET_COUNT - 1;
        update_max_ptr(base_ptr + sizeof(list_t));
        list_init(&buckets[BUCKET_COUNT - 1]);
        list_push(&buckets[BUCKET_COUNT - 1], (list_t *)base_ptr);
    }

    bucket = bucket_for_request(size + HEADER_SIZE);
    original_bucket = bucket;

    while(bucket + 1 != 0) {
        size_t sz, bytes_needed, i;
        uint8_t *ptr;

        if(!lower_bucket_limit(bucket)) {
            return NULL;
        }

        ptr = (uint8_t *)list_pop(&buckets[bucket]);
        if(!ptr) {
            if(bucket != bucket_limit || bucket == 0) {
                bucket--;
                continue;
            }

            if(!lower_bucket_limit(bucket - 1)) {
                return NULL;
            }
            ptr = (uint8_t *)list_pop(&buckets[bucket]);
        }

        sz = (size_t)1 << (MAX_ALLOC_LOG2 - bucket);
        bytes_needed = bucket < original_bucket ? sz / 2 + sizeof(list_t) : sz;
        if(!update_max_ptr(ptr + bytes_needed)) {
            list_push(&buckets[bucket], (list_t *)ptr);
            return NULL;
        }

        i = node_for_ptr(ptr, bucket);
        if(i != 0) {
            flip_parent_is_split(i);
        }

        while(bucket < original_bucket) {
            i = i * 2 + 1;
            bucket++;
            flip_parent_is_split(i);
            list_push(&buckets[bucket], (list_t *)ptr_for_node(i + 1, bucket));
        }

        *(size_t *)ptr = size;
        return ptr + HEADER_SIZE;
    }

    return NULL;
}

void *malloc(size_t size)
{
    return _malloc(size);
}

void *valloc(size_t size)
{
    return aligned_alloc(getpagesize(), size);
}

int posix_memalign(void **res, size_t align, size_t len)
{
    unsigned char *mem, *new, *end;
    size_t header, footer;

    if((align & -align) != align)
        return 1;
    if(len > SIZE_MAX - align)
        return 2;

    if(align <= 4 * sizeof(size_t)) {
        if(!(mem = _malloc(len)))
            return 3;
        *res = mem;
        return 0;
    }

    if(!(mem = _malloc(len + align - 1)))
        return 4;

    header = ((size_t *)mem)[-1];
    end = mem + (header & -8);
    footer = ((size_t *)end)[-2];
    new = (void *)((uintptr_t)mem + align - 1 & -align);

    if(!(header & 7)) {
        ((size_t *)new)[-2] = ((size_t *)mem)[-2] + (new - mem);
        ((size_t *)new)[-1] = ((size_t *)mem)[-1] - (new - mem);
        *res = new;
        return 0;
    }

    ((size_t *)mem)[-1] = header & 7 | new - mem;
    ((size_t *)new)[-2] = footer & 7 | new - mem;
    ((size_t *)new)[-1] = header & 7 | end - new;
    ((size_t *)end)[-2] = footer & 7 | end - new;

    if(new != mem)
        free(mem);
    *res = new;
    return 0;
}

void *aligned_alloc(size_t align, size_t size)
{
    void *res = _malloc(align + size);
    int val = posix_memalign((void *)&res, align, size);
    if(val)
        return NULL;
    return res;
}

[[gnu::weak]] void *aligned_malloc(size_t align, size_t size)
{
    return aligned_alloc(align, ((size / align) + 1) * align);
}

void free(void *ptr)
{
    size_t bucket, i;

    if(!ptr) {
        return;
    }

    ptr = (uint8_t *)ptr - HEADER_SIZE;
    bucket = bucket_for_request(*(size_t *)ptr + HEADER_SIZE);
    i = node_for_ptr((uint8_t *)ptr, bucket);

    while(i != 0) {
        flip_parent_is_split(i);

        if(parent_is_split(i) || bucket == bucket_limit) {
            break;
        }

        list_remove((list_t *)ptr_for_node(((i - 1) ^ 1) + 1, bucket));
        i = (i - 1) / 2;
        bucket--;
    }

    list_push(&buckets[bucket], (list_t *)ptr_for_node(i, bucket));
}
