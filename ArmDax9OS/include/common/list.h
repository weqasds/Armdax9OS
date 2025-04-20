#pragma once 
#include <common/macro.h>
#include <common/types.h>
//链表结构
typedef struct list_head
{
    struct list_head * prev;
    struct list_head * next;
}list_head;
//
typedef struct hlist_node {
    struct hlist_node *next;
    struct hlist_node **pprev; /* the field that points to this node */
}hlist_node;
typedef struct hlist_head {
    struct hlist_node *next;
}hlist_head;
//初始化链表
static inline void init_list_head(struct list_head *list){
    list->next = list;
    list->prev = list;
}
//添加节点到头部
static inline void list_add(struct list_head *new_val, struct list_head *head)
{
        new_val->next = head->next;
        new_val->prev = head;
        head->next->prev = new_val;
        head->next = new_val;
}
//添加节点到尾部
static inline void list_append(struct list_head *new_val, struct list_head *head)
{
        struct list_head *tail = head->prev;
        return list_add(new_val, tail);
}
//删除节点
static inline void list_del(struct list_head *node)
{
        node->prev->next = node->next;
        node->next->prev = node->prev;
}
//判断链表是否为空
static inline bool list_empty(struct list_head *head)
{
        /* When this thing happens, it means someone at the middle of operation
         */
        // BUG_ON((head->prev == head) != (head->next == head));
        return head->next == head;
}

#define next_container_of_safe(obj, type, field)               \
        ({                                                     \
                typeof(obj) __obj = (obj);                     \
                (__obj ? container_of_safe(                    \
                         ((__obj)->field).next, type, field) : \
                         NULL);                                \
        })

#define list_entry(ptr, type, field) container_of(ptr, type, field)

#define for_each_in_list(elem, type, field, head)            \
        for (elem = container_of((head)->next, type, field); \
             &((elem)->field) != (head);                     \
             elem = container_of(((elem)->field).next, type, field))

#define __for_each_in_list_safe(elem, tmp, type, field, head) \
        for (elem = container_of((head)->next, type, field),  \
            tmp = next_container_of_safe(elem, type, field);  \
             &((elem)->field) != (head);                      \
             elem = tmp, tmp = next_container_of_safe(tmp, type, field))

#define for_each_in_list_safe(elem, tmp, field, head) \
        __for_each_in_list_safe (elem, tmp, typeof(*elem), field, head)


static inline void init_hlist_head(struct hlist_head *head)
{
    head->next = nullptr;
}
static inline void init_hlist_node(struct hlist_node *node)
{
    node->next = nullptr;
    node->pprev = nullptr;
}

static inline void hlist_add(struct hlist_node *new_val, struct hlist_head *head)
{
        new_val->next = head->next;
        new_val->pprev = &head->next;
        if (head->next)
                head->next->pprev = &new_val->next;
        head->next = new_val;
}

static inline void hlist_del(struct hlist_node *node)
{
        if (node->next)
                node->next->pprev = node->pprev;
        *node->pprev = node->next;
}

static inline bool hlist_empty(struct hlist_head *head)
{
        return head->next == NULL;
}
#define hlist_entry(ptr, type, field) container_of(ptr, type, field)

#define __for_each_in_hlist(elem, type, field, head)                    \
        for (elem = container_of_safe((head)->next, type, field); elem; \
             elem = elem ? container_of_safe(                           \
                            ((elem)->field).next, type, field) :        \
                           NULL)

#define for_each_in_hlist(elem, field, head) \
        __for_each_in_hlist (elem, typeof(*elem), field, head)

#define __for_each_in_hlist_safe(elem, tmp, type, field, head)    \
        for (elem = container_of_safe((head)->next, type, field), \
            tmp = next_container_of_safe(elem, type, field);      \
             elem;                                                \
             elem = tmp, tmp = next_container_of_safe(elem, type, field))

#define for_each_in_hlist_safe(elem, tmp, field, head) \
        __for_each_in_hlist_safe (elem, tmp, typeof(*elem), field, head)

