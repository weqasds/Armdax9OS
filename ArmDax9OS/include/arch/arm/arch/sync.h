#pragma once
#include <common/types.h>

//所有同步指令

/* 内存屏障指令 */
#define COMPILER_BARRIER() __asm__ volatile("" ::: "memory")
//指令同步屏障 instruction sync barrier
#define ARM_ISB() __asm__ volatile("isb" ::: "memory")
//数据内存屏障data memory barrier
#define ARM_DMB(opt) __asm__ volatile("dmb " #opt ::: "memory")
//数据同步屏障data sync barrier
#define ARM_DSB(opt) __asm__ volatile("dsb " #opt ::: "memory")

//send event
#define sev() __asm__ volatile("sev" ::: "memory")
//wait event
#define wfe() __asm__ volatile("wfe" : : : "memory")
//wait interrupt
#define wfi() __asm__ volatile("wfi" : : : "memory")

#define mb() ARM_DSB(sy)
#define rmb() ARM_DSB(ld)
#define wmb() ARM_DSB(st)

#define smp_mb() ARM_DMB(ish)
#define smp_rmb() ARM_DMB(ishld)
#define smp_wmb() ARM_DMB(ishst)

#define dma_rmb() ARM_DMB(oshld)
#define dma_wmb() ARM_DMB(oshst)
/* 原子加载/存储 */
static inline u32 arm_ldar32(volatile u32 *addr) {
    u32 val;
    __asm__ volatile ("ldar %w0, [%1]" : "=r"(val) : "r"(addr));
    return val;
}

static inline u64 arm_ldar64(volatile u64 *addr) {
    u64 val;
    __asm__ volatile ("ldar %0, [%1]" : "=r"(val) : "r"(addr));
    return val;
}

static inline void arm_stlr32(volatile u32 *addr, u32 val) {
    __asm__ volatile ("stlr %w1, [%0]" :: "r"(addr), "r"(val) : "memory");
}

static inline void arm_stlr64(volatile u64 *addr, u64 val) {
    __asm__ volatile ("stlr %1, [%0]" :: "r"(addr), "r"(val) : "memory");
}

/* 原子交换 - 使用LDXR/STXR优化实现 */
static inline u32 arm_swp32(volatile u32 *ptr, u32 newval) {
    u32 oldval;
    u32 tmp;
    __asm__ volatile (
        "1: ldaxr %w0, [%2]\n"
        "   stlxr %w1, %w3, [%2]\n"
        "   cbnz %w1, 1b"
        : "=&r"(oldval), "=&r"(tmp), "+Q"(*ptr)
        : "r"(newval)
        : "memory");
    return oldval;
}

static inline u64 arm_swp64(volatile u64 *ptr, u64 newval) {
    u64 oldval;
    u32 tmp;
    __asm__ volatile (
        "1: ldaxr %0, [%2]\n"
        "   stlxr %w1, %3, [%2]\n"
        "   cbnz %w1, 1b"
        : "=&r"(oldval), "=&r"(tmp), "+Q"(*ptr)
        : "r"(newval)
        : "memory");
    return oldval;
}

/* 原子比较交换 - 返回操作结果 */
static inline u32 arm_cas32(volatile u32 *ptr, u32 *oldval, u32 newval) {
    u32 tmp;
    u32 res;
    __asm__ volatile (
        "mov %w3, #1\n"
        "ldaxr %w0, [%1]\n"
        "cmp %w0, %w2\n"
        "b.ne 1f\n"
        "stlxr %w3, %w4, [%1]\n"
        "1:\n"
        "cset %w3, eq"
        : "=&r"(tmp), "+r"(ptr), "+r"(oldval), "=&r"(res)
        : "r"(newval)
        : "memory", "cc");
    return res;
}

/* 原子比较交换 - 返回旧值 */
static inline u32 arm_cas_val32(volatile u32 *ptr, u32 compare, u32 newval) {
    u32 oldval;
    u32 tmp;
    __asm__ volatile (
        "1: ldaxr %w0, [%2]\n"
        "   cmp %w0, %w3\n"
        "   b.ne 2f\n"
        "   stlxr %w1, %w4, [%2]\n"
        "   cbnz %w1, 1b\n"
        "2:"
        : "=&r"(oldval), "=&r"(tmp), "+Q"(*ptr)
        : "r"(compare), "r"(newval)
        : "memory", "cc");
    return oldval;
}

static inline u64 arm_cas64(volatile u64 *ptr, u64 *oldval, u64 newval) {
    u64 tmp;
    u64 res;
    __asm__ volatile (
        "mov %w3, #1\n"
        "ldaxr %0, [%1]\n"
        "cmp %0, %2\n"
        "b.ne 1f\n"
        "stlxr %w3, %4, [%1]\n"
        "1:\n"
        "cset %w3, eq"
        : "=&r"(tmp), "+r"(ptr), "+r"(oldval), "=&r"(res)
        : "r"(newval)
        : "memory", "cc");
    return res;
}

static inline u64 arm_cas_val64(volatile u64 *ptr, u64 compare, u64 newval) {
    u64 oldval;
    u32 tmp;
    __asm__ volatile (
        "1: ldaxr %0, [%2]\n"
        "   cmp %0, %3\n"
        "   b.ne 2f\n"
        "   stlxr %w1, %4, [%2]\n"
        "   cbnz %w1, 1b\n"
        "2:"
        : "=&r"(oldval), "=&r"(tmp), "+Q"(*ptr)
        : "r"(compare), "r"(newval)
        : "memory", "cc");
    return oldval;
}

/* 原子算术操作 */
static inline u32 arm_fetch_add32(volatile u32 *ptr, u32 val) {
    u32 oldval, tmp;
    __asm__ volatile (
        "1: ldxr %w0, [%2]\n"
        "add %w1, %w0, %w3\n"
        "stxr %w1, %w1, [%2]\n"
        "cbnz %w1, 1b"
        : "=&r"(oldval), "=&r"(tmp), "+r"(ptr)
        : "r"(val)
        : "memory");
    return oldval;
}

static inline u64 arm_fetch_add64(volatile u64 *ptr, u64 val) {
    u64 oldval, tmp;
    __asm__ volatile (
        "1: ldxr %0, [%2]\n"
        "add %1, %0, %3\n"
        "stxr %w1, %1, [%2]\n"
        "cbnz %w1, 1b"
        : "=&r"(oldval), "=&r"(tmp), "+r"(ptr)
        : "r"(val)
        : "memory");
    return oldval;
}

/* 特殊原子操作 */
static inline u32 arm_test_and_set32(volatile u32 *ptr) {
    return arm_swp32(ptr, 1);
}

static inline u64 arm_test_and_set64(volatile u64 *ptr) {
    return arm_swp64(ptr, 1);
}

static inline void arm_clrex() {
    __asm__ volatile("clrex" ::: "memory");
}
