#include <common/types.h>
#include <irq/ipi.h>
#include <irq/irq.h>
#include <arch/arm/machine/smp.h>
#include <arch/arm/sync.h>
#include <common/kprintf.h>
#include <arch/arm/plat/raspi3/machine.h>
#include <arch/arm/machine/registers.h>

volatile char cpu_status[PLAT_CPU_NUM] = {cpu_hang};


/// @brief 根据启动标志初始化smp
/// @param boot_flag 启动标志
void enable_smp(paddr_t boot_flag)
{
    cpu_status[get_cpu_id()] = cpu_run;
    
}

static inline u32 get_gpu_id(void) {
    u32 cpu_id=0;
    asm volatile("mrs %0, tpidr_el1" : "=r"(cpu_id));
    return cpu_id;
}


/// @brief 获取mpidr寄存器值
/// @param  
static inline u64 smp_get_mpidr(void)
{
    u64 mpidr;
    asm volatile("mrs %0, mpidr_el1" : "=r"(mpidr));
    return mpidr;
}
