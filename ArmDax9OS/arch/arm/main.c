#include<common/types.h>
#include<common/lock.h>
#include<common/kprintf.h>
#include<common/types.h> 
#include <arch/arm/plat/raspi3/machine.h>
#include<common/vars.h>
#include<irq/irq.h>
char kernel_stack[PLAT_CPU_NUM][KERNEL_STACK_SIZE];


/// @brief 内核启动入口
/// @param bootflag 
void main(paddr_t bootflag){}

/* 从核启动入口函数 */
void secondary_start(void)
{
    // 设置当前核心状态为运行
    int cpuid = get_cpuid();
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