#pragma once 
#include <common/types.h>
#include <common/lock.h>
#include <common/list.h>
#include <stdbool.h>

#define SLAB_INIT_SIZE (2 * 1024 * 1024) // 2M
/* order range: [SLAB_MIN_ORDER, SLAB_MAX_ORDER] */
#define SLAB_MIN_ORDER (5)  // 2^5=32
#define SLAB_MAX_ORDER (11) // 2^11=2048

// slab内存块结构
struct slab_block {
    void *free_list_head; 
    struct list_head list;  // 链表节点
    int order;              // 块大小阶数
};
// slab控制结构
struct slab_control {
    spin_lock lock;          // 保护该结构的锁
    void *memory_base;        // 内存池起始地址
    size_t total_size;        // 总内存大小
    struct list_head free_lists[SLAB_MAX_ORDER - SLAB_MIN_ORDER + 1]; // 各阶空闲链表
};

// 声明内存区域起始地址
extern u64 __slab_mem_start;
extern u64 __slab_mem_end;

//函数声明
void slab_init(void);
//分配slab
void *slab_alloc(u64 size);
void *slab_alloc_nolock(u64 size);
//释放slab
void slab_free(void *ptr);
static int is_block_free(struct slab_block *blk);

