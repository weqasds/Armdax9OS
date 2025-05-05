#pragma once
#include <common/vars.h>
#include <common/types.h>

/* 内存页大小定义 */
#define PAGE_SHIFT      12
#define PAGE_SIZE       (1 << PAGE_SHIFT)  /* 4KB */
//页掩码
#define PAGE_MASK       (~(PAGE_SIZE - 1))

/* 架构相关内存初始化 */
void arch_mm_init(void);
void mm_init(void);

/* 内存管理核心接口 */
void *mm_alloc_pages(u64 order);
void mm_free_pages(void *addr, u64 order);
void *mm_get_phys_addr(void *vaddr);
void *mm_get_virt_addr(void *paddr);

/* 地址空间检查 */
//是否是用户空间地址
static inline bool_ is_user_addr(vaddr_t vaddr){
    return vaddr < KBASE;
}
//是否是内核空间地址
static inline bool_ is_kernel_addr(vaddr_t vaddr){
    return vaddr > KBASE;
}
//给定地址与size检查这片范围地址是否处于用户空间地址
static inline bool_ is_user_addr_range(vaddr_t vaddr,u64 size){
    return (vaddr + size >= vaddr) && is_user_addr(vaddr + size);
}

extern void memcpy(void *dst, const void *src, size_t size);
extern void memmove(void *dst, const void *src, size_t size);
extern void memset(void *dst, char ch, size_t size);

/* 内存统计信息 */
struct mm_stats {
    u64 total_pages;
    u64 free_pages;
    u64 used_pages;
};
void mm_get_stats(struct mm_stats *stats);

/* 页表操作接口 */
int mm_map_page(void *vaddr, void *paddr, u64 flags);
int mm_unmap_page(void *vaddr);
void *mm_query_mapping(void *vaddr);
void mm_switch_pgd(void *pgd);
void mm_invalidate_tlb(void *vaddr, size_t size);

/* 缓存一致性接口 */
void mm_flush_dcache(void *addr, size_t size);
void mm_invalidate_icache(void *addr, size_t size);
void mm_flush_dcache_all(void);


void set_page_table(paddr_t pgtbl);
