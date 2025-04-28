#pragma once
#include <common/types.h>
#include <common/list.h>
#include <common/lock.h>
/*
 * Supported Order: [0, BUDDY_MAX_ORDER).
 * The max allocated size (continous physical memory size) is
 * 2^(BUDDY_MAX_ORDER - 1) * 4K, i.e., 16M.
 */
#define BUDDY_PAGE_SIZE (0x1000)
#define BUDDY_MAX_ORDER (14UL)
#define BUDDY_PAGE_SIZE_ORDER (12)
#define PHYS_MEM_POOLS 3
extern struct phys_mem_pool global_mem_pool[PHYS_MEM_POOLS];
extern paddr_t physmene_map[PHYS_MEM_POOLS][2];
extern int physmem_map_num;

struct page
{
    unsigned long flags;   // 页状态标志 0未使用 1已使用
    int order;             // 所属的order
    void* slab;            //分配器
    struct list_head list; // free list
    struct phys_mem_pool *pool; //指向所属物理内存池
};

struct free_list
{
    /// @brief 将要释放的页链表
    struct list_head free_list;
    /// @brief 未释放的页面
    unsigned long nr_free;
};

/*
    物理内存池
    管理内存中所有物理页
*/
struct phys_mem_pool
{
    /// @brief 内存池的起始地址
    vaddr_t pool_start_addr;
    /// @brief 内存池大小
    unsigned long pool_mem_size;
    /// @brief 页表数组
    struct page *page_metadata;
    /// @brief 互斥锁
    spin_lock buddy_lock;
    /// @brief 空闲链表数组
    struct free_list free_lists[BUDDY_MAX_ORDER];
    /// @brief test kernel
    unsigned long pool_phys_page_num;
};


/* 初始化伙伴系统 */
int buddy_init(struct phys_mem_pool *pool, struct page *page_start, vaddr_t start, unsigned long size);

/* 分配2^order个连续物理页 */
struct page *buddy_alloc_pages(struct phys_mem_pool *pool, unsigned int order);

/* 释放分配的页 */
void buddy_free_pages(struct phys_mem_pool *pool, struct page *page);

/* 获取页的伙伴 */
struct page *get_buddy(struct phys_mem_pool *pool, struct page *page);

/* 分割页块 */
void split_page(struct phys_mem_pool *pool, struct page *page, unsigned int order);

/* 合并页块 */
void merge_page(struct phys_mem_pool *pool, struct page *page, unsigned int order);

/* 获取页的物理地址 */
paddr_t page_to_paddr(struct phys_mem_pool *pool, struct page *page);

/* 通过物理地址获取页 */
struct page *paddr_to_page(struct phys_mem_pool *pool, paddr_t paddr);

/* 通过页获取虚拟地址 */
void *page_to_vaddr(struct page *page);

/* 通过虚拟地址找页 */
struct page *vaddr_to_page(void *ptr);

/* 获取当前空闲页数 */
unsigned long buddy_get_free_pages(struct phys_mem_pool *pool);

/* 获取总页数 */
unsigned long buddy_get_total_pages(struct phys_mem_pool *pool);

/* 获取最大order */
unsigned int buddy_get_max_order(void);

/* 获取页的order */
unsigned int page_get_order(struct page *page);

/* 设置页的order */
void page_set_order(struct page *page, unsigned int order);

/* 检查页是否空闲 */
int page_is_free(struct page *page);

/* 设置页状态 */
void page_set_free(struct page *page, int free);