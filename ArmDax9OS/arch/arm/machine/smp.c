#include <common/types.h>
#include <irq/ipi.h>
#include <irq/irq.h>
#include <arch/arm/machine/smp.h>
#include <arch/arm/sync.h>
#include <common/kprintf.h>
#include <arch/arm/plat/raspi3/machine.h>
#include <arch/arm/machine/registers.h>

volatile char cpu_status[PLAT_CPU_NUM] = {cpu_hang};



/* 初始化所有CPU核心 */
void smp_init(void)
{
    // 初始化主核状态
    cpu_status[0] = cpu_run;

    // 初始化从核
    for (int i = 1; i < PLAT_CPU_NUM; i++) {
        cpu_status[i] = cpu_hang;
    }
}
u64 ctr_el0;

/* 启动从核 */
void smp_boot_secondary(int cpu, void (*entry)(void))
{
    if (cpu >= PLAT_CPU_NUM || cpu == 0)
        return;

    // 设置从核入口地址
    write_sysreg(entry, __smp_up_entry);

    // 发送处理器间中断(IPI)唤醒从核
    send_ipi(cpu, IPI_WAKEUP);

    // 等待从核启动完成
    while (cpu_status[cpu] != cpu_run)
        cpu_relax();
}

/* 停止从核 */
void smp_stop_secondary(int cpu)
{
    if (cpu >= PLAT_CPU_NUM || cpu == 0)
        return;

    // 发送停止请求
    send_ipi(cpu, IPI_STOP_CPU);

    // 等待从核停止
    while (cpu_status[cpu] != cpu_hang)
        cpu_relax();
}
