/*
    配置并实现mmu
*/
#include "mmu.h"
#include "const.h"
typedef unsigned long u64;//UL
typedef unsigned long long u64l;//ULL
typedef unsigned int u32;//U
///树莓派3b 物理内存为1GB
//起始地址
#define PHYSMEME_START (0x0UL)
//外设基准地址
#define PERIPHERAL_BASE (0x3F000000UL)
//结束地址
#define PHYSMEM_END     (0x40000000UL)


#define PTE_ENTRIES_NUM 512
#define PTP_SIZE 4096
#define ALIGN(n) __attribute__((__aligned__(n)))
//ttbr 0 l0 寄存器配置
u64 boot_ttbr0_l0[PTE_ENTRIES_NUM] ALIGN(PTP_SIZE);
u64 boot_ttbr0_l1[PTE_ENTRIES_NUM] ALIGN(PTP_SIZE);
u64 boot_ttbr0_l2[PTE_ENTRIES_NUM] ALIGN(PTP_SIZE);

u64 boot_ttbr1_l0[PTE_ENTRIES_NUM] ALIGN(PTP_SIZE);
u64 boot_ttbr1_l1[PTE_ENTRIES_NUM] ALIGN(PTP_SIZE);
u64 boot_ttbr1_l2[PTE_ENTRIES_NUM] ALIGN(PTP_SIZE);

#define IS_VALID (1UL << 0)//0b1
#define IS_TABLE (1UL << 1)//0b10
#define UXN            (0x1UL << 54)
#define ACCESSED       (0x1UL << 10)
//非全局位
#define NG             (0x1UL << 11)
#define INNER_SHARABLE (0x3UL << 8)
#define NORMAL_MEMORY  (0x0UL << 2)
#define DEVICE_MEMORY  (0x1UL << 2)

#define SIZE_2M (2UL * 1024 * 1024)
//获取对应等级索引 ttbr l0 l1 l2
#define GET_L0_INDEX(x) (((x) >> (12 + 9 + 9 + 9)) & 0x1ff)
#define GET_L1_INDEX(x) (((x) >> (12 + 9 + 9)) & 0x1ff)
#define GET_L2_INDEX(x) (((x) >> (12 + 9)) & 0x1ff)

//初始化页表
void init_boot_pt(void){
    // 配置ttbr0（用户空间）
    // L0 -> L1
    boot_ttbr0_l0[GET_L0_INDEX(PHYSMEME_START)] = (u64)boot_ttbr0_l1 | IS_TABLE | IS_VALID | NG;
    // L1 -> L2
    boot_ttbr0_l1[GET_L1_INDEX(PHYSMEME_START)] = (u64)boot_ttbr0_l2 | IS_TABLE | IS_VALID | NG;;
    
    // 配置ttbr1（内核空间）
    boot_ttbr1_l0[GET_L0_INDEX(KENERAL_VADDR)] = (u64)boot_ttbr1_l1 | IS_TABLE | IS_VALID;
    boot_ttbr1_l1[GET_L1_INDEX(KENERAL_VADDR)] = (u64)boot_ttbr1_l2 | IS_TABLE | IS_VALID;

    // 映射普通内存（0x0 ~ 0x3F000000）
    for(u64 paddr = PHYSMEME_START; paddr < PERIPHERAL_BASE; paddr += SIZE_2M){
        // 用户空间映射（ttbr0）
        u64 l2_index = GET_L2_INDEX(paddr);
        boot_ttbr0_l2[l2_index] = paddr | IS_VALID 
                                        | NORMAL_MEMORY 
                                        | ACCESSED 
                                        | NG 
                                        | UXN
                                        | INNER_SHARABLE;

        // 内核空间映射（ttbr1，虚拟地址高位映射）
        u64 vaddr = KENERAL_VADDR | paddr; // 内核虚拟地址=物理地址+固定偏移
        u64 l1_idx = GET_L1_INDEX(vaddr);
        u64 l2_idx = GET_L2_INDEX(vaddr);
        if(!(boot_ttbr1_l1[l1_idx] & IS_TABLE)){
            boot_ttbr1_l1[l1_idx] = (u64)&boot_ttbr1_l2[l1_idx*PTE_ENTRIES_NUM] | IS_TABLE;
        }
        boot_ttbr1_l2[l2_idx] = paddr | IS_VALID | NORMAL_MEMORY | INNER_SHARABLE;
    }

    // 映射外设内存（0x3F000000 ~ 0x40000000）
    for(u64 paddr = PERIPHERAL_BASE; paddr < PHYSMEM_END; paddr += SIZE_2M){
        // 用户空间映射
        u64 l2_index = GET_L2_INDEX(paddr);
        boot_ttbr0_l2[l2_index] = paddr | IS_VALID | DEVICE_MEMORY | NG | UXN;
        
        // 内核空间映射
        u64 vaddr = KENERAL_VADDR | paddr;
        u64 l1_idx = GET_L1_INDEX(vaddr);
        u64 l2_idx = GET_L2_INDEX(vaddr);
        if(!(boot_ttbr1_l1[l1_idx] & IS_TABLE)){
            boot_ttbr1_l1[l1_idx] = (u64)&boot_ttbr1_l2[l1_idx*PTE_ENTRIES_NUM] | IS_TABLE;
        }
        boot_ttbr1_l2[l2_idx] = paddr | IS_VALID 
                                      | DEVICE_MEMORY 
                                      | UXN 
                                      | ACCESSED 
                                      | NG;
    }
}