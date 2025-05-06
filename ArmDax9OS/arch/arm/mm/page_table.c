#include <arch/arm/mm/page_table.h>
#include <arch/arm/sync.h>
#include <arch/arm/mmu.h>
#include <common/macro.h>
#include <mm/slab.h>
#include <arch/arm/sync.h>
#include <common/types.h>
#include <mm/mm.h>
#include <arch/arm/mmu.h>
#include <common/error.h>
#define PAGE_MASK (~(PAGE_SIZE - 1))
#define GET_PADDR_IN_PTE(entry) \
        (((u64)entry->table.addr) << PAGE_SHIFT)
#define GET_NEXT_PTP(entry) phys_to_virt(GET_PADDR_IN_PTE(entry))

#define NORMAL_PTP (0)
#define BLOCK_PTP  (1)
/* 设置TTBR0寄存器 */
//in arch/arm/mm/page_table.S
extern void set_ttbr0_el1(paddr_t);
/* 获取TTBR0寄存器值 */
//in arch/arm/mm/page_table.S
extern paddr_t get_ttbr0_el1(void);


void set_page_table(paddr_t pgtbl)
{
    set_ttbr0_el1(pgtbl);
}

/* 获取下一级页表页 */
int get_next_ptp(ptp_t *current_ptp, u64 index, vaddr_t va, ptp_t **next_ptp, pte_t **pte, bool_ alloc)
{
    u32 index = 0;
    pte_t *entry;
    if (current_ptp == NULL)
                return -ENOMAPPING;
    switch (index)
    {
    case 0:
        index = GET_L0_INDEX(va);
        break;
    case 1:
        index = GET_L1_INDEX(va);
        break;
    case 2:
        index = GET_L2_INDEX(va);
        break;
    case 3:
        index = GET_L3_INDEX(va);
        break;
    default:
        BUG_ON(1);
    }
    entry=&(current_ptp->entries[index]);
    if (IS_PTE_INVALID(entry->value)) {
        if (alloc == false)
        {
            return -ENOMAPPING;
        }
        else
        {
            /* alloc a new page table page */
            ptp_t *new_ptp;
            paddr_t new_ptp_paddr;
            pte_t new_pte_val;

            /* alloc a single physical page as a new page table page
             */
            new_ptp = get_pages(0);
            BUG_ON(new_ptp == NULL);
            memset((void *)new_ptp, 0, PAGE_SIZE);
            new_ptp_paddr = virt_to_phys((vaddr_t)new_ptp);

            new_pte_val.value = 0;
            new_pte_val.table.valid = 1;
            new_pte_val.table.type = 1;
            new_pte_val.table.addr = new_ptp_paddr >> PAGE_SHIFT;

            /* same effect as: cur_ptp->ent[index] = new_pte_val; */
            entry->value = new_pte_val.value;
        }
    }
    *next_ptp = (ptp_t *)GET_NEXT_PTP(entry);
        *pte = entry;
        if (IS_PTE_TABLE(entry->value))
                return NORMAL_PTP;
        else
                return BLOCK_PTP;
}

/* 查询虚拟地址映射 */
pte_t *find_pte(u64 vaddr) {
    ptp_t *pgd = (ptp_t *)(get_ttbr0_el1() & PAGE_MASK);
    if (!pgd) return NULL;

    ptp_t *pud = get_next_ptp(pgd, pgd_index(vaddr));
    if (!pud) return NULL;

    ptp_t *pmd = get_next_ptp(pud, pud_index(vaddr));
    if (!pmd) return NULL;

    ptp_t *pt = get_next_ptp(pmd, pmd_index(vaddr));
    if (!pt) return NULL;

    return &pt->entries[pte_index(vaddr)];
}

/* 映射4KB页范围 */
int map_4k_page(u64 vaddr, u64 paddr, u64 attr) {
    ptp_t *pgd = (ptp_t *)(get_ttbr0_el1() & PAGE_MASK);
    if (!pgd) return -1;

    ptp_t *pud = get_next_ptp(pgd, pgd_index(vaddr));
    if (!pud) {
        pud = kmalloc(sizeof(ptp_t));
        if (!pud) return -1;
        memset(pud, 0, sizeof(ptp_t));
        pgd->entries[pgd_index(vaddr)].value = ((u64)pud >> PAGE_SHIFT) | PTE_VALID | PTE_TABLE;
    }

    ptp_t *pmd = get_next_ptp(pud, pud_index(vaddr));
    if (!pmd) {
        pmd = kmalloc(sizeof(ptp_t));
        if (!pmd) return -1;
        memset(pmd, 0, sizeof(ptp_t));
        pud->entries[pud_index(vaddr)].value = ((u64)pmd >> PAGE_SHIFT) | PTE_VALID | PTE_TABLE;
    }

    ptp_t *pt = get_next_ptp(pmd, pmd_index(vaddr));
    if (!pt) {
        pt = kmalloc(sizeof(ptp_t));
        if (!pt) return -1;
        memset(pt, 0, sizeof(ptp_t));
        pmd->entries[pmd_index(vaddr)].value = ((u64)pt >> PAGE_SHIFT) | PTE_VALID | PTE_TABLE;
    }

    pte_t *pte = &pt->entries[pte_index(vaddr)];
    pte->value = (paddr >> PAGE_SHIFT) | PTE_VALID | attr;
    return 0;
}

/* 取消映射4KB页范围 */
int unmap_4k_page(u64 vaddr) {
    pte_t *pte = find_pte(vaddr);
    if (!pte || !(pte->value & PTE_VALID))
        return -1;

    pte->value = 0;
    return 0;
}

/* 映射2MB大页范围 */
int map_2m_page(u64 vaddr, u64 paddr, u64 attr) {
    ptp_t *pgd = (ptp_t *)(get_ttbr0_el1() & PAGE_MASK);
    if (!pgd) return -1;

    ptp_t *pud = get_next_ptp(pgd, pgd_index(vaddr));
    if (!pud) {
        pud = kmalloc(sizeof(ptp_t));
        if (!pud) return -1;
        memset(pud, 0, sizeof(ptp_t));
        pgd->entries[pgd_index(vaddr)].value = ((u64)pud >> PAGE_SHIFT) | PTE_VALID | PTE_TABLE;
    }

    ptp_t *pmd = get_next_ptp(pud, pud_index(vaddr));
    if (!pmd) {
        pmd = kmalloc(sizeof(ptp_t));
        if (!pmd) return -1;
        memset(pmd, 0, sizeof(ptp_t));
        pud->entries[pud_index(vaddr)].value = ((u64)pmd >> PAGE_SHIFT) | PTE_VALID | PTE_TABLE;
    }

    pte_t *pte = &pmd->entries[pmd_index(vaddr)];
    pte->value = (paddr >> PAGE_SHIFT) | PTE_VALID | PTE_BLOCK | attr;
    return 0;
}

/* 取消映射2MB大页范围 */
int unmap_2m_page(u64 vaddr) {
    ptp_t *pgd = (ptp_t *)(get_ttbr0_el1() & PAGE_MASK);
    if (!pgd) return -1;

    ptp_t *pud = get_next_ptp(pgd, pgd_index(vaddr));
    if (!pud) return -1;

    ptp_t *pmd = get_next_ptp(pud, pud_index(vaddr));
    if (!pmd) return -1;

    pte_t *pte = &pmd->entries[pmd_index(vaddr)];
    if (!(pte->value & PTE_VALID))
        return -1;

    pte->value = 0;
    return 0;
}


/* 设置PTE标志位(4k) */
void set_pte_flags_4k(pte_t *pte, u64 flags) {
    if (!pte) return;
    pte->value |= flags;
}

/* 设置PTE标志位(2m) */
void set_pte_flags_2m(pte_t *pte, u64 flags) {
    if (!pte || !(pte->value & PTE_VALID)) return;
    
    // 确保设置BLOCK类型并清除TABLE类型
    flags |= PTE_BLOCK;
    flags &= ~PTE_TABLE;
    
    // 原子修改标志位
    pte->value = (pte->value & (PTE_PFN_MASK | PTE_VALID)) | flags;
    
    // 内存屏障保证修改立即可见
    dsb(ishst);
    isb();
}

/* 递归释放页表 */
static void free_ptp_recursive(ptp_t *ptp, int level) {
    if (!ptp || level >= 4) return;

    for (int i = 0; i < PTRS_PER_PTE; i++) {
        pte_t *pte = &ptp->entries[i];
        if (pte->value & PTE_VALID && pte->value & PTE_TABLE) {
            ptp_t *next_ptp = (ptp_t *)(pte->table.addr << PAGE_SHIFT);
            free_ptp_recursive(next_ptp, level + 1);
        }
    }
    kfree(ptp);
}

/* 释放整个页表 */
void free_page_table(ptp_t *ptp) {
    if (!ptp) {
        ptp = (ptp_t *)(get_ttbr0_el1() & PAGE_MASK);
    }
    free_ptp_recursive(ptp, 0);
}
