#pragma once 

#ifndef NR_CPUS
#define NR_CPUS 4
#endif
#include <common/types.h>
/*
 * 每CPU变量宏实现
 * ---------------------------
 * 完整生产级实现，支持SMP系统
 */

/* 声明外部定义的CPU偏移量数组 */
extern unsigned long __per_cpu_offset[NR_CPUS];

/* 获取指定CPU的偏移量 */
static inline unsigned long per_cpu_offset(int cpu)
{
    return __per_cpu_offset[cpu];
}

/* 每CPU变量属性标记 */
#ifdef CONFIG_SMP
#define __percpu __attribute__((section(".data..percpu")))
#else
#define __percpu
#endif

/* 分配每CPU变量空间 */
#define alloc_percpu(type) \
    (typeof(type) __percpu *)__alloc_percpu(sizeof(type), __alignof__(type))
/* 获取指定CPU的变量指针 */
#define per_cpu_ptr(ptr, cpu) \
    ({ \
        unsigned long __ptr = (unsigned long)(ptr); \
        (typeof(ptr))(__ptr + per_cpu_offset(cpu)); \
    })

/* 底层获取当前CPU指针（不带抢占保护） */
#define __get_cpu_ptr(ptr) \
    per_cpu_ptr(ptr, smp_processor_id())

/* 获取当前CPU的变量指针（带抢占保护） */
#define this_cpu_ptr(ptr) \
    ({ \
        __typeof__(ptr) __p; \
        preempt_disable(); \
        __p = __get_cpu_ptr(ptr); \
        preempt_enable(); \
        __p; \
    })

/* 静态定义每CPU变量 */
#define DEFINE_PER_CPU(type, name) \
    __typeof__(type) name __percpu

/* 声明外部每CPU变量 */
#define DECLARE_PER_CPU(type, name) \
    extern DEFINE_PER_CPU(type, name)

/*
 * 每CPU变量访问宏
 * ---------------------------
 * 安全访问每CPU变量的快捷方式
 */

/* 获取当前CPU的变量值 */
#define this_cpu_read(var) \
    (*this_cpu_ptr(&(var)))

/* 设置当前CPU的变量值 */
#define this_cpu_write(var, val) \
    (*this_cpu_ptr(&(var)) = (val))

/* 对当前CPU变量进行加法操作 */
#define this_cpu_add(var, val) \
    (this_cpu_write(var, this_cpu_read(var) + (val)))