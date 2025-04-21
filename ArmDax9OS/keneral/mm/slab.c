#include <mm/slab.h>
#include <common/types.h>
#include <common/lock.h>
#include <common/macro.h>
#include <common/list.h>
#include <common/kprintf.h>

//全局slab控制器
static struct slab_control slab_ctrl;

// 初始化slab分配器
void slab_init(void) {
    // 初始化锁
    slab_ctrl.lock.spin_slock = 0; // 初始化自旋锁
    
    // 设置内存参数
    slab_ctrl.memory_base = (void *)__slab_mem_start; // 需要链接器提供
    slab_ctrl.total_size = SLAB_INIT_SIZE;
    
    // 初始化各阶空闲链表
    for (int i = 0; i <= (SLAB_MAX_ORDER - SLAB_MIN_ORDER); i++) {
        INIT_LIST_HEAD(&slab_ctrl.free_lists[i]);
    }
    
    // 将整个内存初始化为最大块并加入空闲链表
    struct slab_block *blk = (struct slab_block *)slab_ctrl.memory_base;
    blk->order = SLAB_MAX_ORDER;
    list_add(&blk->list, &slab_ctrl.free_lists[SLAB_MAX_ORDER - SLAB_MIN_ORDER]);
}

// 分配内存块
void *slab_alloc(u64 size) {
    // 计算所需order
    int req_order = get_order(size);
    if (req_order < SLAB_MIN_ORDER) req_order = SLAB_MIN_ORDER;
    if (req_order > SLAB_MAX_ORDER) return NULL;
    
    spin_lock_acquire(&slab_ctrl.lock);
    
    // 查找合适的内存块
    int curr_order = req_order;
    while (curr_order <= SLAB_MAX_ORDER) {
        if (!list_empty(&slab_ctrl.free_lists[curr_order - SLAB_MIN_ORDER])) {
            // 找到可用块
            struct slab_block *blk = list_entry(
                slab_ctrl.free_lists[curr_order - SLAB_MIN_ORDER].next,
                struct slab_block, list);
            list_del(&blk->list);
            
            // 如果块太大则分裂
            while (blk->order > req_order) {
                blk->order--;
                struct slab_block *buddy = get_buddy(blk);
                buddy->order = blk->order;
                list_add(&buddy->list, 
                    &slab_ctrl.free_lists[buddy->order - SLAB_MIN_ORDER]);
            }
            
            spin_lock_release(&slab_ctrl.lock);
            return (void *)blk;
        }
        curr_order++;
    }
    
    spin_lock_release(&slab_ctrl.lock);
    return NULL; // 内存不足
}


void *slab_alloc_nolock(u64 size) {
    // 计算所需order
    int req_order = get_order(size);
    if (req_order < SLAB_MIN_ORDER) req_order = SLAB_MIN_ORDER;
    if (req_order > SLAB_MAX_ORDER) return NULL;
    
    // 查找合适的内存块
    int curr_order = req_order;
    while (curr_order <= SLAB_MAX_ORDER) {
        if (!list_empty(&slab_ctrl.free_lists[curr_order - SLAB_MIN_ORDER])) {
            // 找到可用块
            struct slab_block *blk = list_entry(
                slab_ctrl.free_lists[curr_order - SLAB_MIN_ORDER].next,
                struct slab_block, list);
            list_del(&blk->list);
            
            // 如果块太大则分裂
            while (blk->order > req_order) {
                blk->order--;
                struct slab_block *buddy = get_buddy(blk);
                buddy->order = blk->order;
                list_add(&buddy->list, 
                    &slab_ctrl.free_lists[buddy->order - SLAB_MIN_ORDER]);
            }
            
            return (void *)blk;
        }
        curr_order++;
    }
    
    return NULL; // 内存不足
}

// 释放内存块
void slab_free(void *ptr) {
    if (!ptr) return;
    
    struct slab_block *blk = (struct slab_block *)ptr;
    spin_lock_acquire(&slab_ctrl.lock);
    
    // 尝试合并伙伴块
    while (blk->order < SLAB_MAX_ORDER) {
        struct slab_block *buddy = get_buddy(blk);
        
        // 检查伙伴块是否空闲且同阶
        if (buddy->order != blk->order || 
            is_block_free(buddy) == 0) {
            break;
        }
        
        // 合并伙伴块
        list_del(&buddy->list);
        blk = MIN(blk, buddy);
        blk->order++;
    }
    
    // 将合并后的块加入空闲链表
    list_add(&blk->list, 
        &slab_ctrl.free_lists[blk->order - SLAB_MIN_ORDER]);
    
    spin_lock_release(&slab_ctrl.lock);
}

// 辅助函数：计算所需order
static u64 get_order(u64 size) {
    int order = SLAB_MIN_ORDER;
    u64 chunk_size = (1 << order);
    
    while (chunk_size < size) {
        order++;
        chunk_size <<= 1;
    }
    
    return order;
}
//将阶数直接转换为内存块大小数字
static inline u64 order_to_size(u64 order){
    return 1UL << order;
}

// 辅助函数：获取伙伴块
static struct slab_block *get_buddy(struct slab_block *blk) {
    u64 size = (1 << blk->order);
    return (struct slab_block *)((u64)blk ^ size);
}

// 辅助函数：检查块是否空闲
static int is_block_free(struct slab_block *blk) {
    // 检查块是否在对应阶的空闲链表中
    struct slab_block *pos;
    int order = blk->order;
    
    for_each_in_list(pos, struct slab_block, list, &slab_ctrl.free_lists[order - SLAB_MIN_ORDER]) {
        if (pos == blk) {
            return 1; // 找到匹配块，返回空闲状态
        }
    }
    return 0; // 未找到匹配块
}
