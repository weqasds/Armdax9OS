#include <sched/sched.h>
#include<common/kprintf.h>
#include<common/vars.h>
#include<common/macro.h>
#include<common/types.h>
#include<common/lock.h>
#include<arch/arm/machine/smp.h>
#include<arch/arm/machine/pmu.h>
#include<arch/arm/mm/page_table.h>
#include<mm/mm.h>
#include<lib/uart.h>
#include<arch/arm/plat/raspi3/machine.h>
#include<irq/irq.h>
#include<object/object.h>
ALIGN(STACK_ALIGNMENT);
char kernel_stack[PLAT_CPU_NUM][KERNEL_STACK_SIZE];


/// @brief 内核启动入口
/// @param bootflag
void main(paddr_t bootflag)
{
    u32 ret=0;
    kernel_lock_init();
    kinfo("[Arm9daxOS] lock init finished\n");
    BUG_ON(ret != 0);

    /* Init uart: no need to init the uart again */
    uart_init();
    kinfo("[Arm9daxOS] uart init finished\n");
    mm_init();
    arch_irq_init();
}

/* 从核启动入口函数 */
void secondary_start(void)
{
    // 设置当前核心状态为运行
    int cpuid = get_cpu_id();
    cpu_status[cpuid] = cpu_run;

    // 初始化核心本地存储
    init_per_cpu();

    // 启用中断
    enable_irq();

    // 从核主循环
    while (1) {
        // 空闲时进入低功耗状态
        if (cpu_status[cpuid] == cpu_idle) {
            wfi();
        }
        // 处理IPI中断
        handle_ipi();
    }
}