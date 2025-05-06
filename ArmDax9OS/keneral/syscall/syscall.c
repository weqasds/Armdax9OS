#include <common/types.h>
#include <lib/uart.h>
#include <mm/kmalloc.h>
#include <mm/mm.h>
#include <common/kprintf.h>
#include <object/object.h>
#include <object/cap_group.h>
#include <sched/sched.h>
#include <irq/irq.h>
#include <common/lock.h>
#include <lib/uart.h>
#include <arch/arm/machine/smp.h>
#include <arch/arm/machine/pmu.h>
#include "syscall_num.h"
// 默认系统调用处理函数
static void sys_undefined(void)
{
    kprintf("Undefined syscall!\n");
    return -1;
}
static void sys_putc(char ch)
{
    uart_putc(ch);
}
u32 sys_getc(void)
{
        return uart_recv();
}
// 基础系统调用实现
static long sys_exit(int status)
{
    // 进程退出逻辑
    return 0;
}

static long sys_yield(void)
{
    // 主动让出CPU
    schedule();
    return 0;
}

static long sys_get_time(void)
{
    // 获取系统时间
    return get_system_time();
}

// Other系统调用实现
static long sys_irq_register(int irq_num, void (*handler)(void))
{
    // 注册IRQ处理函数
    return register_irq_handler(irq_num, handler);
}

static long sys_irq_wait(int irq_num)
{
    // 等待IRQ发生
    return wait_for_irq(irq_num);
}

static long sys_irq_ack(int irq_num)
{
    // 确认IRQ处理完成
    return ack_irq(irq_num);
}

/* Syscalls for perfromance benchmark */
void sys_perf_start(void)
{
        kdebug("Disable TIMER\n");
        plat_disable_timer();
}

void sys_perf_end(void)
{
        kdebug("Enable TIMER\n");
        plat_enable_timer();
}

extern arch_flush_cache(u64, s64, int);
extern u64 plat_get_current_tick(void);
int sys_cache_flush(u64 start, s64 len, int op_type)
{
        arch_flush_cache(start, len, op_type);
        return 0;
}

u64 sys_get_current_tick(void)
{
        return plat_get_current_tick();
}

// 系统调用表
const void* syscall_table[NR_SYSCALL] = {
    [SYS_exit] = sys_exit,
    [SYS_yield] = sys_yield,
    [SYS_get_time] = sys_get_time,
    [SYS_nanosleep] = sys_undefined,
    [SYS_get_tid] = sys_undefined,
    [SYS_get_pid] = sys_undefined,
    [SYS_get_cpu_id] = sys_undefined,
    // 填充其他系统调用...
    [SYS_irq_register] = sys_irq_register,
    [SYS_irq_wait] = sys_irq_wait,
    [SYS_irq_ack] = sys_irq_ack,
    [SYS_user_fault_register] = sys_undefined,
    [SYS_user_fault_map] = sys_undefined,
    [SYS_putc] = sys_putc,
    [SYS_getc] = sys_getc,
    [SYS_perf_start] = sys_perf_start,
    [SYS_perf_end] = sys_perf_end,
    [SYS_cache_flush] = sys_cache_flush,
    [SYS_get_current_tick] = sys_get_current_tick
};