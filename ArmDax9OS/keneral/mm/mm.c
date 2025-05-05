#include <mm/mm.h>
#include <common/types.h>
#include <mm/buddy.h>
#include <mm/slab.h>
#include <common/kprintf.h>

extern void parse_mem_map(void);
struct phys_mem_pool global_mem_pool[PHYS_MEM_POOLS];
paddr_t physmem_map[PHYS_MEM_POOLS][2];
int physmem_map_num;
void mm_init(void)
{
    vaddr_t free_mem_start = 0;
    vaddr_t free_mem_end = 0;
    struct page *page_meta_start = NULL;
    u64 npages = 0;
    u64 start_vaddr = 0;

    physmem_map_num = 0;
    parse_mem_map();

    if (physmem_map_num == 1) {
            free_mem_start = phys_to_virt(physmem_map[0][0]);
            free_mem_end = phys_to_virt(physmem_map[0][1]);

            npages = (free_mem_end - free_mem_start)
                     / (PAGE_SIZE + sizeof(struct page));
            start_vaddr =
                    ROUND_UP(free_mem_start + npages * sizeof(struct page),
                             PAGE_SIZE);
            kdebug("[Arm9daxOS] mm: free_mem_start is 0x%lx, free_mem_end is 0x%lx\n",
                   free_mem_start,
                   free_mem_end);

            page_meta_start = (struct page *)free_mem_start;
            kdebug("page_meta_start: 0x%lx, real_start_vaddr: 0x%lx\n"
                   "npages: 0x%lx, page_meta_size: 0x%lx\n",
                   page_meta_start,
                   start_vaddr,
                   npages,
                   sizeof(struct page));

            /* buddy alloctor for managing physical memory */
            init_buddy(
                    &global_mem_pool[0], page_meta_start, start_vaddr, npages);
    } else {
            BUG("Unsupported physmem_map_num\n");
    }

    /* slab alloctor for allocating small memory regions */
    init_slab();
}