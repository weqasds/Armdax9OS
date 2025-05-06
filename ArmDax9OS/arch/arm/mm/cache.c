#include <common/types.h>
#include <arch/arm/machine/registers.h>
#include <common/asm.h>
#include <common/kprintf.h>
#include <arch/arm/sync.h>

#define CACHE_LINE_LENGTH  64
#define CACHE_CLEAN         1
#define CACHE_INVALIDATE    2
#define CACHE_CLEAN_AND_INV 3
#define SYNC_IDCACHE        4

#define ICACHE_POLICY_VPIPT  0
#define ICACHE_POLICY_AIVIVT 1
#define ICACHE_POLICY_VIPT   2
#define ICACHE_POLICY_PIPT   3
int dczva_line_size = 0;

long read_dczid(void)
{
    long val;
    asm volatile("mrs %0, dczid_el0\n\t" : "=r"(val));
    return val;
}

/* 清理数据缓存范围 */
void dcache_clean_range(vaddr_t start, size_t size)
{
    vaddr_t end = start + size;
    vaddr_t line_size = dczva_line_size;
    vaddr_t align_mask = ~(line_size - 1);
    
    /* 对齐到缓存行边界 */
    start &= align_mask;
    
    mb();
    
    for (; start < end; start += CACHE_LINE_LENGTH) {
        __asm__ volatile("dc cvau, %0" :: "r"(start));
    }
    
    mb();
    isb();
}

/* 无效化数据缓存范围 */
void dcache_invalidate_range(vaddr_t start, size_t size)
{
    vaddr_t end = start + size;
    vaddr_t line_size = dczva_line_size;
    vaddr_t align_mask = ~(line_size - 1);
    
    /* 对齐到缓存行边界 */
    start &= align_mask;
    
    mb();
    
    for (; start < end; start += CACHE_LINE_LENGTH) {
        __asm__ volatile("dc ivau, %0" :: "r"(start));
    }
    
    mb();
    isb();
}

/* 清理并无效化数据缓存范围 */
void dcache_clean_invalidate_range(vaddr_t start, size_t size)
{
    vaddr_t end = start + size;
    vaddr_t line_size = dczva_line_size;
    vaddr_t align_mask = ~(line_size - 1);
    
    /* 对齐到缓存行边界 */
    start &= align_mask;
    
    mb();
    
    for (; start < end; start += CACHE_LINE_LENGTH) {
        __asm__ volatile("dc civac, %0" :: "r"(start));
    }
    
    mb();
    isb();
}

/* 无效化指令缓存 */
void icache_invalidate_all(void)
{
    mb();
    __asm__ volatile("ic ialluis");
    isb();
}

/* 初始化缓存子系统 */
void cache_init(void)
{
    detect_cache_line_size();
    
    /* 初始时无效化所有缓存 */
    dcache_invalidate_range(0, ~0);
    icache_invalidate_all();
    
    kinfo("Cache subsystem initialized\n");
    kinfo("DC ZVA line size: %d bytes\n", dczva_line_size);
}

/* 获取缓存行大小 */
static void detect_cache_line_size(void)
{
    u64 ctr_el0;
    __asm__ volatile("mrs %0, ctr_el0" : "=r"(ctr_el0));
    
    /* 提取DminLine字段(字节) */
    dczva_line_size = 4 << ((ctr_el0 >> 16) & 0xF);
    
    kdebug("Detected DC ZVA line size: %d bytes\n", dczva_line_size);
}

/**
 * @brief 统一的缓存操作接口
 * @param start 起始虚拟地址
 * @param len 操作长度
 * @param op_type 操作类型:
 *   CACHE_CLEAN - 清理数据缓存
 *   CACHE_INVALIDATE - 无效化数据缓存
 *   CACHE_CLEAN_AND_INV - 清理并无效化数据缓存
 *   SYNC_IDCACHE - 同步指令和数据缓存
 */
void arch_flush_cache(u64 start, s64 len, int op_type)
{
    /* 参数检查 */
    if (len <= 0 || op_type < CACHE_CLEAN || op_type > SYNC_IDCACHE) {
        kwarn("Invalid cache flush parameters: start=0x%lx len=%ld op=%d\n",
              start, len, op_type);
        return;
    }

    /* 确保缓存行大小已初始化 */
    if (unlikely(dczva_line_size == 0)) {
        detect_cache_line_size();
    }

    switch (op_type) {
    case CACHE_CLEAN:
        dcache_clean_range(start, len);
        break;
    case CACHE_INVALIDATE:
        dcache_invalidate_range(start, len);
        break;
    case CACHE_CLEAN_AND_INV:
        dcache_clean_invalidate_range(start, len);
        break;
    case SYNC_IDCACHE:
        /* 先清理数据缓存，再无效化指令缓存 */
        dcache_clean_range(start, len);
        isb();
        icache_invalidate_all();
        break;
    default:
        BUG("Invalid op_type:%d\n", op_type);
        break;
    }
}
