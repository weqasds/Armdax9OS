#pragma once
#include <common/types.h>
//in arch/arm/irq/irq.S
extern void flush_dcache_area(u64 addr,u64 size);
extern void disable_irq(void);
extern void enable_irq(void);

//in arch/arm/tools.S
extern void put32(u64 addr, u32 data);
extern unsigned int get32(u64 addr);
