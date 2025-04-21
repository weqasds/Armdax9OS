#include <arch/arm/arch/mm/page_table.h>
#include <common/macro.h>
#include <mm/slab.h>
#include <arch/arm/arch/sync.h>
#include <common/types.h>
#include <mm/mm.h>
#define PAGE_MASK (~(PAGE_SIZE - 1))

/* 设置TTBR0寄存器 */
extern void set_ttbr0_el1(paddr_t);
/* 获取TTBR0寄存器值 */
extern paddr_t get_ttbr0_el1(void);

/* 获取下一级页表页 */
ptp_t *get_next_ptp(ptp_t *current_ptp, u64 index) {
    if (!current_ptp || index >= PTRS_PER_PTE)
        return NULL;

    pte_t *pte = &current_ptp->entries[index];
    if (!(pte->value & PTE_VALID) || !(pte->value & PTE_TABLE))
        return NULL;

    return (ptp_t *)(pte->table.addr << PAGE_SHIFT);
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
void set_page_table(paddr_t pgtbl)
{
    set_ttbr0_el1(pgtbl);
}

/* 设置PTE标志位 */
void set_pte_flags(pte_t *pte, u64 flags) {
    if (!pte) return;
    pte->value |= flags;
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
