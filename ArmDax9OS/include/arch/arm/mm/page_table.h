#pragma once
#include "common/types.h"

/* 内存属性类型 */
#define MT_DEVICE_nGnRnE  0x0
#define MT_DEVICE_nGnRE   0x1
#define MT_DEVICE_GRE     0x2
#define MT_NORMAL_NC      0x3
#define MT_NORMAL         0x4
#define MT_NORMAL_WT      0x5

/* 内存属性索引 */
#define MAIR_ATTR(attr, idx) ((attr) << ((idx) * 8))

/* 页表属性位 */
#define PTE_VALID         (1UL << 0)    // 有效位
#define PTE_TABLE         (1UL << 1)    // 表描述符
#define PTE_BLOCK         (0UL << 1)    // 块描述符
#define PTE_AF            (1UL << 10)   // 访问标志
#define PTE_nG            (1UL << 11)   // 非全局
#define PTE_PXN           (1UL << 53)   // 特权执行不可
#define PTE_UXN           (1UL << 54)   // 用户执行不可
#define PTE_AP_RW         (0UL << 6)    // 读写权限
#define PTE_AP_RO         (1UL << 7)    // 只读权限
#define PTE_AP_USER       (1UL << 6)    // 用户可访问
#define PTE_SH_INNER      (3UL << 8)    // 内部可共享
#define PTE_SH_OUTER      (2UL << 8)    // 外部可共享
#define PTE_SH_NONE       (0UL << 8)    // 不可共享


/* Access flag bit. */
#define AARCH64_MMU_ATTR_PAGE_AF_ACCESSED (1)

/* Present (valid) bit. */
#define AARCH64_MMU_PTE_INVALID_MASK (1 << 0)
/* Table bit: whether the next level is aonther pte or physical memory page. */
#define AARCH64_MMU_PTE_TABLE_MASK (1 << 1)
#define IS_PTE_INVALID(pte)        (!((pte)&AARCH64_MMU_PTE_INVALID_MASK))
#define IS_PTE_TABLE(pte)          (!!((pte)&AARCH64_MMU_PTE_TABLE_MASK))

/* 页表层级相关宏 */
#define PAGE_SHIFT 12
/* PFN mask for 48-bit address space (bits [47:12]) */
#define PTE_PFN_MASK    (0x0000FFFFFFFFF000ULL)
#define PTE_PFN_MASK    ((1ULL << 48) - 1) & ~((1ULL << 12) - 1)
#define PAGE_SIZE        (1UL << PAGE_SHIFT)
#define PMD_SHIFT        21
#define PUD_SHIFT        30
#define PGDIR_SHIFT      39

#define PTRS_PER_PTE     512
#define PTRS_PER_PMD     512
#define PTRS_PER_PUD     512
#define PTRS_PER_PGD     512

#define PMD_SIZE         (1UL << PMD_SHIFT)
#define PUD_SIZE         (1UL << PUD_SHIFT)
#define PGDIR_SIZE       (1UL << PGDIR_SHIFT)

#define PMD_MASK         (~(PMD_SIZE - 1))
#define PUD_MASK         (~(PUD_SIZE - 1))
#define PGDIR_MASK       (~(PGDIR_SIZE - 1))

/* 页表索引计算 */
#define pgd_index(addr)  (((addr) >> PGDIR_SHIFT) & (PTRS_PER_PGD - 1))
#define pud_index(addr)  (((addr) >> PUD_SHIFT) & (PTRS_PER_PUD - 1))
#define pmd_index(addr)  (((addr) >> PMD_SHIFT) & (PTRS_PER_PMD - 1))
#define pte_index(addr)  (((addr) >> PAGE_SHIFT) & (PTRS_PER_PTE - 1))

/* 页表条目类型 */
typedef union {
    struct {
        u64 valid:1;      // 有效位
        u64 type:1;       // 0=块, 1=表
        u64 attr_index:3; // 内存属性索引
        u64 ns:1;         // 安全状态
        u64 ap:2;         // 访问权限
        u64 sh:2;         // 共享属性
        u64 af:1;         // 访问标志
        u64 ng:1;         // 非全局
        u64 res0:18;      // 保留
        u64 addr:28;      // 物理地址
        u64 res1:4;       // 保留
        u64 pxd:1;        // PXN/UXN
        u64 cont:1;       // 连续条目
    } table;                   // 表描述符

    struct {
        u64 valid:1;
        u64 type:1;
        u64 attr_index:3;
        u64 ns:1;
        u64 ap:2;
        u64 sh:2;
        u64 af:1;
        u64 ng:1;
        u64 res0:9;
        u64 addr:28;      // 1GB物理地址
        u64 res1:4;
        u64 pxd:1;
        u64 cont:1;
    } l1_block;                // 1GB块描述符

    struct {
        u64 valid : 1;       // [0]     页表项是否有效
        u64 type : 1;        // [1]     0=block, 1=table（本层为 block）
        u64 attr_index : 3;  // [2:4]   MAIR索引（内存类型属性）
        u64 ns : 1;          // [5]     Non-secure（0=secure, 1=non-secure）
        u64 ap : 2;          // [6:7]   访问权限（EL0/EL1 可读/写控制）
        u64 sh : 2;          // [8:9]   Shareability 属性
        u64 af : 1;          // [10]    Access Flag
        u64 ng : 1;          // [11]    Not Global bit
        u64 res0 : 4;        // [12:15] 保留位（SBZ）
        u64 addr : 28;       // [16:43] Block物理地址[47:21]（对齐 2MB）
        u64 res1 : 4;        // [44:47] 保留位
        u64 dbm : 1;         // [51]    Dirty Bit Modifier
        u64 cont : 1;        // [52]    连续映射
        u64 pxd : 1;         // [53]    Privileged Execute Never（PXN）
        u64 uxn : 1;         // [54]    User Execute Never（UXN）
        u64 pbha : 4;        // [55:58] Page-Based Hardware Attributes
        u64 res2 : 5;        // [59:63] 保留/忽略位
    } l2_block;

    struct {
        u64 valid:1;
        u64 type:1;
        u64 attr_index:3;
        u64 ns:1;
        u64 ap:2;
        u64 sh:2;
        u64 af:1;
        u64 ng:1;
        u64 res0:4;
        u64 addr:28;      // 4KB物理地址
        u64 res1:4;
        u64 dbm:1;
        u64 cont:1;
        u64 pxd:1;
        u64 uxn:1;
        u64 pbha:1;
        u64 res2:5;
    } l3_page;                 // 4KB页描述符

    u64 value;
} pte_t;

/* 页表页结构 */
typedef struct {
    pte_t entries[PTRS_PER_PTE];
} ptp_t;


/* 页表操作函数声明 */
//pgtbl: page table 

/*未来实现*/
//void set_ttbr1_el1(paddr_t pgtbl);
//void set_ttbr0_el2(paddr_t pgtbl);
//void set_ttbr0_el3(paddr_t pgtbl);


int get_next_ptp(ptp_t *current_ptp, u64 index, vaddr_t va, ptp_t **next_ptp, pte_t **pte, bool_ alloc);
pte_t *find_pte(u64 vaddr);

void set_pte_flags_4k(pte_t *pte, u64 flags);
void set_pte_flags_2m(pte_t *pte, u64 flags);
void free_page_table(ptp_t *ptp);

