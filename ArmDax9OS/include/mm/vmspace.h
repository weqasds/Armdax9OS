#pragma once 
#include <common/types.h>
#include <common/list.h>
#include <common/radix.h>
#include <common/rbtree.h>
#include <common/lock.h>
#include <arch/arm/mmu.h>
//虚拟内存区域，进程的各种段
struct vmregion
{
    struct list_head list_node;
    struct rb_node tree_node;
    list_head mapping_list_node;
    //内存区域所属地址空间
    struct vmspace *vmspace;
    vaddr_t start;
    size_t size;
    size_t offset;
    vmr_prop_t perm;

};

/*
    虚拟地址空间结构
    用于描述每个进程的地址空间
*/
struct vmspace
{
    struct list_head vmr_list;
    struct rb_root vmr_tree;
    void *pgtbl;
    unsigned long pcid;
    struct spin_lock vmspace_lock;
    struct spin_lock pgtbl_lock;

    unsigned char history_cpus[PLAT_CPU_NUM];

    struct vmregion *heap_boundary_vmr;
    long rss;

};

