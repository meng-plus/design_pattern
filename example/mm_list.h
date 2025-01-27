#ifndef _MM_LIST_H_
#define _MM_LIST_H_

#include <stddef.h>
#include <stdint.h>

/* 链表节点定义 */
typedef struct mm_list
{
    struct mm_list *next;
    struct mm_list *prev;
} mm_list_t;

/* 初始化链表 */
#define MM_LIST_INIT(name) {&(name), &(name)}

/* 定义和初始化链表头 */
#define MM_LIST_HEAD(name) \
    mm_list_t name = MM_LIST_INIT(name)


/* 遍历链表 */
#define mm_list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/* 遍历链表安全版本（在遍历过程中可以删除节点） */
#define mm_list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); pos = n, n = pos->next)

/* 获取包含某成员的结构体指针 */
#define MM_LIST_ENTRY(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

#define mm_list_for_each_entry(pos, head, member)                    \
    for (pos = MM_LIST_ENTRY((head)->next, rt_typeof(*pos), member); \
         &pos->member != (head);                                     \
         pos = MM_LIST_ENTRY(pos->member.next, rt_typeof(*pos), member))


#define mm_list_for_each_entry_safe(pos, n, head, member)                \
    for (pos = MM_LIST_ENTRY((head)->next, rt_typeof(*pos), member),     \
        n    = MM_LIST_ENTRY(pos->member.next, rt_typeof(*pos), member); \
         &pos->member != (head);                                         \
         pos = n, n = MM_LIST_ENTRY(n->member.next, rt_typeof(*n), member))


/* 初始化链表节点 */
static inline void mm_list_init(mm_list_t *list)
{
    list->next = list;
    list->prev = list;
}

/* 插入节点到指定节点之后 */
static inline void mm_list_insert_after(mm_list_t *list, mm_list_t *node)
{
    node->next       = list->next;
    node->prev       = list;
    list->next->prev = node;
    list->next       = node;
}

/* 插入节点到指定节点之前 */
static inline void mm_list_insert_before(mm_list_t *list, mm_list_t *node)
{
    node->next       = list;
    node->prev       = list->prev;
    list->prev->next = node;
    list->prev       = node;
}
/* 检查链表中是否已经包含某个元素 */
static inline int mm_list_contains(mm_list_t *list, mm_list_t *node, int (*cmp)(mm_list_t *node1, mm_list_t *node2))
{
    mm_list_t *pos;
    mm_list_for_each(pos, list)
    {
        if (cmp(pos, node) == 0) // 如果元素相等，返回真
        {
            return 1;            // 已经包含
        }
    }
    return 0; // 不包含
}

/* 插入节点到链表中，避免重复 */
static inline int mm_list_insert_unique(mm_list_t *list, mm_list_t *node, int (*cmp)(mm_list_t *node1, mm_list_t *node2))
{
    // 如果链表中已经存在该元素，就不插入
    if (mm_list_contains(list, node, cmp))
    {
        return -1; // 返回 -1 表示元素已存在
    }

    // 如果不存在，插入节点
    mm_list_insert_after(list, node);
    return 0; // 插入成功
}

/* 从链表中移除节点 */
static inline void mm_list_remove(mm_list_t *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    mm_list_init(node); // 防止野指针
}

/* 判断链表是否为空 */
static inline int mm_list_is_empty(const mm_list_t *list)
{
    return list->next == list;
}
/* 获得链表长度 */
static inline unsigned int mm_list_len(const mm_list_t *list)
{
    unsigned int len   = 0;
    const mm_list_t *p = list;
    while (p->next != list)
    {
        p = p->next;
        len++;
    }

    return len;
}

#endif /* _MM_LIST_H_ */
