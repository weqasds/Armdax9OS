#pragma once 
#include <arch/arm/plat/raspi3/machine.h>
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
    //内存区域起始地址
    vaddr_t start;
    /// @brief 内存区域大小
    size_t size;
    /// @brief 偏移量
    size_t offset;
    /// @brief 区域权限属性
    vmr_prop_t perm;
    /// @brief 私有页表
    struct list_head cow_private_page;
};

/*
    虚拟地址空间结构
    用于描述每个进程的地址空间
*/
struct vmspace
{
    /// @brief vmregion内存区域
    struct list_head vmr_list;
    /// @brief vmregion根节点
    struct rb_root vmr_tree;
    /// @brief 根页面
    void *pgtbl;
    /* Address space ID for avoiding TLB conflicts */
    unsigned long pcid;
    /// @brief 地址空间锁
    struct spin_lock vmspace_lock;
    /// @brief 页面锁
    struct spin_lock pgtbl_lock;

    unsigned char history_cpus[PLAT_CPU_NUM];

    struct vmregion *heap_boundary_vmr;
    long rss;

};

