#pragma once
#include <common/cpuinfo.h>
#include <slab.h>

/*
 * 每CPU数据区实现
 */

/* 每CPU变量偏移量数组 */
unsigned long __per_cpu_offset[NR_CPUS] __read_mostly;

/* 实际分配每CPU内存的函数 */
void __percpu *__alloc_percpu(size_t size, size_t align)
{
    void __percpu *ptr;
    int cpu;
    
    /* 分配主指针 */
    ptr = kmalloc(sizeof(void *) * NR_CPUS, GFP_KERNEL);
    if (!ptr)
        return NULL;
    
    /* 为每个CPU分配独立区域 */
    for_each_possible_cpu(cpu) {
        void *p = kmalloc_node(size, GFP_KERNEL, cpu_to_node(cpu));
        if (!p) {
            /* 分配失败，清理已分配的内存 */
            for_each_possible_cpu(cpu2) {
                if (cpu2 == cpu)
                    break;
                kfree(per_cpu_ptr(ptr, cpu2));
            }
            kfree(ptr);
            return NULL;
        }
        *((void **)per_cpu_ptr(ptr, cpu)) = p;
    }
    
    return ptr;
}

/* 初始化每CPU偏移量 */
void __init setup_per_cpu_areas(void)
{
    unsigned long size = __per_cpu_end - __per_cpu_start;
    int cpu;
    
    /* 为每个CPU分配并复制数据区 */
    for_each_possible_cpu(cpu) {
        void *ptr = alloc_bootmem_node(NODE_DATA(cpu_to_node(cpu)), size);
        memcpy(ptr, __per_cpu_start, size);
        __per_cpu_offset[cpu] = ptr - (void *)__per_cpu_start;
    }
}