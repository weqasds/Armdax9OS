#include <common/types.h>
#include <common/macro.h>
#include <mm/buddy.h>
#include <common/debug.h>
#include <common/lock.h>
#include <mm/memory.h>
#include <common/list.h>
#include <common/kprintf.h>

/* 初始化伙伴系统 */
int buddy_init(struct phys_mem_pool *pool, struct page *page_start, vaddr_t start, unsigned long size)
{
    if (!pool || !size)
        return -1;

    // 计算总页数
    unsigned long page_num = size / BUDDY_PAGE_SIZE;
    if (page_num == 0)
        return -1;

    // 初始化内存池基本信息
    pool->pool_start_addr = start;
    pool->pool_mem_size = size;
    pool->pool_phys_page_num = page_num;

    // 分配页元数据数组
    pool->page_metadata = (struct page *)kmalloc(page_num * sizeof(struct page));
    if (!pool->page_metadata)
        return -1;

    // 初始化自旋锁
    spin_lock_init(&pool->buddy_lock);

    // 初始化空闲链表
    for (int i = 0; i < BUDDY_MAX_ORDER; i++)
    {
        init_list_head(&pool->free_lists[i].free_list);
        pool->free_lists[i].nr_free = 0;
    }

    // 将所有页初始化为最大order的空闲页
    unsigned long max_order = min(BUDDY_MAX_ORDER - 1, ilog2(page_num));
    for (unsigned long i = 0; i < page_num;)
    {
        unsigned long chunk_size = 1UL << max_order;
        if (i + chunk_size > page_num)
        {
            max_order--;
            continue;
        }

        struct page *page = &pool->page_metadata[i];
        page_set_order(page, max_order);
        page_set_free(page, 1);
        list_add(&page->list, &pool->free_lists[max_order].free_list);
        pool->free_lists[max_order].nr_free++;
        i += chunk_size;
    }

    return 0;
}

/* 分配2^order个连续物理页 */
struct page *buddy_alloc_pages(struct phys_mem_pool *pool, unsigned int order)
{
    if (!pool || order >= BUDDY_MAX_ORDER)
        return NULL;

    spin_lock_acquire(&pool->buddy_lock);

    // 查找满足要求的order
    unsigned int current_order = order;
    while (current_order < BUDDY_MAX_ORDER &&
           list_empty(&pool->free_lists[current_order].free_list))
    {
        current_order++;
    }

    if (current_order >= BUDDY_MAX_ORDER)
    {
        spin_lock_release(&pool->buddy_lock);
        return NULL; // 没有足够的内存
    }

    // 获取页块
    struct list_head *head = &pool->free_lists[current_order].free_list;
    struct page *page = list_entry(head->next, struct page, list);
    list_del_init(&page->list);
    pool->free_lists[current_order].nr_free--;

    // 如果需要分割
    while (current_order > order)
    {
        current_order--;
        split_page(pool, page, current_order);
    }

    page_set_free(page, 0);
    spin_lock_release(&pool->buddy_lock);
    return page;
}

/* 释放分配的页 */
void buddy_free_pages(struct phys_mem_pool *pool, struct page *page)
{
    if (!pool || !page)
        return;

    spin_lock_acquire(&pool->buddy_lock);
    unsigned int order = page_get_order(page);
    page_set_free(page, 1);

    // 尝试合并伙伴块
    while (order < BUDDY_MAX_ORDER - 1)
    {
        struct page *buddy = get_buddy(pool, page);
        if (!buddy || !page_is_free(buddy) || page_get_order(buddy) != order)
            break;

        // 合并伙伴块
        list_del_init(&buddy->list);
        pool->free_lists[order].nr_free--;
        merge_page(pool, page, order);
        order++;
    }

    // 将合并后的块添加到空闲链表
    list_add(&page->list, &pool->free_lists[order].free_list);
    pool->free_lists[order].nr_free++;
    spin_lock_release(&pool->buddy_lock);
}

/* 获取页的伙伴 */
struct page *get_buddy(struct phys_mem_pool *pool, struct page *page)
{
    if (!pool || !page)
        return NULL;

    unsigned long pfn = page - pool->page_metadata;
    unsigned long buddy_pfn = pfn ^ (1UL << page_get_order(page));

    if (buddy_pfn >= pool->pool_phys_page_num)
        return NULL;

    return &pool->page_metadata[buddy_pfn];
}

/* 分割页块 */
void split_page(struct phys_mem_pool *pool, struct page *page, unsigned int order)
{
    if (!pool || !page || order >= BUDDY_MAX_ORDER - 1)
        return;

    unsigned int current_order = page_get_order(page);
    if (order >= current_order)
        return;

    // 设置新order
    unsigned long size = 1UL << order;
    page_set_order(page, order);
    page_set_free(page, 1);

    // 创建伙伴块
    struct page *buddy = &pool->page_metadata[(page - pool->page_metadata) + size];
    page_set_order(buddy, order);
    page_set_free(buddy, 1);

    // 将伙伴块添加到空闲链表
    list_add(&buddy->list, &pool->free_lists[order].free_list);
    pool->free_lists[order].nr_free++;
}

/* 合并页块 */
void merge_page(struct phys_mem_pool *pool, struct page *page, unsigned int order)
{
    if (!pool || !page || order >= BUDDY_MAX_ORDER - 1)
        return;

    // 设置新的order
    page_set_order(page, order + 1);
    page_set_free(page, 1);
}

/* 通过页获取虚拟地址 */
void *page_to_vaddr(struct page *page)
{
    if (!page)
        return NULL;
    struct phys_mem_pool *pool = page->pool;
    unsigned long pfn = page - kernel_pool.page_metadata;
    return (void *)(kernel_pool.pool_start_addr + (pfn << BUDDY_PAGE_SIZE_ORDER));
}

/* 通过虚拟地址找页 */
struct page *vaddr_to_page(void *ptr)
{
    if (!ptr)
        return NULL;

    // 假设虚拟地址与物理地址线性映射
    extern struct phys_mem_pool kernel_pool;
    paddr_t paddr = (paddr_t)ptr;
    return paddr_to_page(&kernel_pool, paddr);
}

/* 获取页的物理地址 */
paddr_t page_to_paddr(struct phys_mem_pool *pool, struct page *page)
{
    if (!pool || !page)
        return 0;

    unsigned long pfn = page - pool->page_metadata;
    return pool->pool_start_addr + (pfn << BUDDY_PAGE_SIZE_ORDER);
}

/* 通过物理地址获取页 */
struct page *paddr_to_page(struct phys_mem_pool *pool, paddr_t paddr)
{
    if (!pool || paddr < pool->pool_start_addr ||
        paddr >= pool->pool_start_addr + pool->pool_mem_size)
        return NULL;

    unsigned long pfn = (paddr - pool->pool_start_addr) >> BUDDY_PAGE_SIZE_ORDER;
    if (pfn >= pool->pool_phys_page_num)
        return NULL;

    return &pool->page_metadata[pfn];
}

/* 获取当前空闲页数 */
unsigned long buddy_get_free_pages(struct phys_mem_pool *pool)
{
    if (!pool)
        return 0;

    unsigned long total = 0;
    for (int i = 0; i < BUDDY_MAX_ORDER; i++)
    {
        total += pool->free_lists[i].nr_free * (1UL << i);
    }
    return total;
}

/* 获取总页数 */
unsigned long buddy_get_total_pages(struct phys_mem_pool *pool)
{
    return pool ? pool->pool_phys_page_num : 0;
}

/* 获取最大order */
unsigned int buddy_get_max_order(void)
{
    return BUDDY_MAX_ORDER;
}

/* 获取页的order */
unsigned int page_get_order(struct page *page)
{
    return page ? page->order : 0;
}

/* 设置页的order */
void page_set_order(struct page *page, unsigned int order)
{
    if (page)
        page->order = order;
}

/* 检查页是否空闲 */
int page_is_free(struct page *page)
{
    return page ? (page->flags & 1) : 0;
}

/* 设置页状态 */
void page_set_free(struct page *page, int free)
{
    if (page)
    {
        if (free)
            page->flags |= 1;
        else
            page->flags &= ~1;
    }
}