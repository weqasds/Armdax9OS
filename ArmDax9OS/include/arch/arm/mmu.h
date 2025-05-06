#pragma once


#ifndef KBASE
#define KBASE              0xFFFFFF0000000000
#define PHYSICAL_ADDR_MASK (40)
#endif // KBASE
#ifndef __ASM__
#include <common/types.h>
typedef u64 vmr_prop_t;
#define VMR_READ    (1 << 0)
#define VMR_WRITE   (1 << 1)
#define VMR_EXEC    (1 << 2)
#define VMR_DEVICE  (1 << 3)
#define VMR_NOCACHE (1 << 4)
int map_4k_page(u64 vaddr, u64 paddr, u64 attr);
int unmap_4k_page(u64 vaddr);
int map_2m_page(u64 vaddr, u64 paddr, u64 attr);
int unmap_2m_page(u64 vaddr);


#define phys_to_virt(x) ((vaddr_t)((paddr_t)(x) + KBASE)) // 物理地址空间到虚拟地址
#define virt_to_phys(x) ((paddr_t)((vaddr_t)(x) - KBASE)) // 虚拟地址到物理地址空间
#endif