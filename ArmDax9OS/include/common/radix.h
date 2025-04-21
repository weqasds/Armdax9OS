#pragma once
#include <common/types.h>
#include <common/lock.h>
#include <common/macro.h>

/* Radix树参数定义 */
#define RADIX_BITS        4       // 每个节点的基数(2^4=16个子节点)
#define RADIX_MAX_LEVELS  8       // 最大层级数
#define RADIX_MASK        ((1 << RADIX_BITS) - 1)

/* Radix树节点结构 */
typedef struct radix_node {
    struct radix_node *children[1 << RADIX_BITS]; // 子节点数组
    void *value;                                  // 存储的值
    struct spin_lock lock;                       // 自旋锁
} radix_node_t;

/* Radix树结构 */
typedef struct radix_tree {
    radix_node_t *root;          // 根节点
    u32 height;                  // 当前高度
    struct spin_lock lock;       // 树级锁
} radix_tree_t;

/* 函数声明 */
void radix_tree_init(radix_tree_t *tree);
void radix_tree_destroy(radix_tree_t *tree);

int radix_tree_insert(radix_tree_t *tree, u64 key, void *value);
void *radix_tree_lookup(radix_tree_t *tree, u64 key);
int radix_tree_delete(radix_tree_t *tree, u64 key);

void *radix_tree_lookup_safe(radix_tree_t *tree, u64 key);
int radix_tree_insert_safe(radix_tree_t *tree, u64 key, void *value);
int radix_tree_delete_safe(radix_tree_t *tree, u64 key);

/* 迭代器相关 */
typedef struct radix_tree_iter {
    radix_node_t *node;
    u64 key;
    u32 level;
} radix_tree_iter_t;

void radix_tree_iter_init(radix_tree_iter_t *iter, radix_tree_t *tree);
void *radix_tree_iter_next(radix_tree_iter_t *iter);
