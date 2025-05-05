#include <common/kprintf.h>
#include <arch/arm/machine/esr.h>
#include <arch/arm/machine/smp.h>
#include "irq_entry.h"
#include <irq/irq.h>
#include <common/types.h>
#include <sched/sched.h>

u8 irq_handle_type[MAX_IRQ_NUM];
