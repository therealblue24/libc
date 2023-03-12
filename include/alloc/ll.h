#ifndef LL_H__
#define LL_H__

#include <stdint.h>
#include <stdlib.h>
#include <detail/internal.h>

#ifndef offsetof
#ifdef __compiler_offsetof
#define offsetof(TYPE, MEMBER) __compiler_offsetof(TYPE, MEMBER)
#else
#define offsetof(TYPE, MEMBER) ((size_t) & ((TYPE *)0)->MEMBER)
#endif
#endif

#ifndef container_of
#ifdef __GNUC__
#ifndef __clang__
#define container_of(ptr, type, member)                        \
	({                                                         \
		const __typeof__(((type *)0)->member) *__mptr = (ptr); \
		(type *)((uintptr_t)__mptr - offsetof(type, member));  \
	})
#else
#define container_of(ptr, type, member) \
	((type *)((uintptr_t)(ptr)-offsetof(type, member)))
#endif
#else
#define container_of(ptr, type, member) \
	((type *)((uintptr_t)(ptr)-offsetof(type, member)))
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct ll_head {
	struct ll_head *next;
	struct ll_head *prev;
} ll_t;

#define list_entry(ptr, type, member) container_of(ptr, type, member)
#define list_first_entry(head, type, member) \
	list_entry((head)->next, type, member)
#define list_for_each(pos, head) \
	for(pos = (head)->next; pos != (head); pos = pos->next)
#define list_for_each_safe(pos, n, head)                  \
	for(pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)
#define list_for_each_entry(pos, head, member)                    \
	for(pos = list_entry((head)->next, __typeof__(*pos), member); \
		&pos->member != (head);                                   \
		pos = list_entry(pos->member.next, __typeof__(*pos), member))
#define list_for_each_entry_safe(pos, n, head, member)            \
	for(pos = list_entry((head)->next, __typeof__(*pos), member), \
	n = list_entry(pos->member.next, __typeof__(*pos), member);   \
		&pos->member != (head);                                   \
		pos = n, n = list_entry(n->member.next, __typeof__(*n), member))
#define ll_head_INIT(name) \
	{                      \
		&(name), &(name)   \
	}
#define LIST_INIT(name) struct ll_head name = ll_head_INIT(name)

static inline void __tr24_list_insert(struct ll_head *n, struct ll_head *prev,
									  struct ll_head *next)
{
	next->prev = n;
	n->next = next;
	n->prev = prev;
	prev->next = n;
}
#define list_insert __cctr24(list_insert)

static inline void __tr24_list_add(struct ll_head *n, struct ll_head *head)
{
	__tr24_list_insert(n, head, head->next);
}
#define list_add __cctr24(list_add)

static inline void __tr24_list_add_tail(struct ll_head *n, struct ll_head *head)
{
	__tr24_list_insert(n, head->prev, head);
}
#define list_add_tail __cctr24(list_add_tail)

static inline void __tr24_list_join_nodes(struct ll_head *prev,
										  struct ll_head *next)
{
	next->prev = prev;
	prev->next = next;
}
#define list_join_nodes __cctr24(list_join_nodes)

static inline void __tr24_list_del(struct ll_head *entry)
{
	__tr24_list_join_nodes(entry->prev, entry->next);
	entry->next = (void *)0;
	entry->prev = (void *)0;
}
#define list_del __cctr24(list_del)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_H__ */
