#pragma once 
#include <common/types.h>
#include <arch/arm/plat/raspi3/machine.h>
#include <common/vars.h>

enum cpu_state{
    cpu_hang = 0,
    cpu_run  = 1,
    cpu_idle = 2,
};
extern volatile char cpu_status[PLAT_CPU_NUM];

void enable_smp(paddr_t boot_flag);
u32 get_gpu_id(void);

/// @brief 获取mpidr寄存器值
/// @param  
u64 smp_get_mpidr(void);
