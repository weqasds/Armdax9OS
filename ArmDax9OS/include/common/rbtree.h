#pragma once
#include <common/types.h>
#include <common/lock.h>

/* 红黑树节点结构（64字节对齐） */
typedef struct rb_node {
    struct rb_node *__parent_color; // 利用指针低位存储颜色信息
    struct rb_node *right;
    struct rb_node *left;
} __attribute__((aligned(64))) rb_node;

/* 红黑树根结构 */
typedef struct rb_root {
    struct rb_node *rb_node;
    struct spin_lock lock;  // 使用完整结构体类型
} rb_root;

/* 颜色标志 */
#define RB_RED      0
#define RB_BLACK    1

/* 通过成员获取包含结构 */
#define rb_entry(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

/* 基本操作宏 */
#define RB_ROOT_INIT { NULL, SPIN_LOCK_INIT }

/* 迭代器宏 */
#define rb_for_each(pos, root) \
    for (pos = rb_first(root); pos; pos = rb_next(pos))

void rb_insert(rb_root *root, rb_node *node);
void rb_erase(rb_root *root, rb_node *node);
rb_node *rb_first(rb_root *root);
rb_node *rb_next(rb_node *node);
rb_node *rb_last(rb_root *root);
rb_node *rb_prev(rb_node *node);
rb_node *rb_find(rb_root *root, unsigned long key);
rb_node *rb_find_ge(rb_root *root, unsigned long key);
rb_node *rb_find_le(rb_root *root, unsigned long key);

/* 旋转操作 */
void rb_rotate_left(rb_root *root, rb_node *node);
void rb_rotate_right(rb_root *root, rb_node *node);

/* 颜色操作 */
void rb_set_color(rb_node *node, int color);
int rb_get_color(rb_node *node);
void rb_set_parent(rb_node *node, rb_node *parent);
rb_node *rb_get_parent(rb_node *node);

/* 替换操作 */
void rb_replace_node(rb_root *root, rb_node *victim, rb_node *new);

/* 验证函数 */
void rb_verify(rb_root *root);