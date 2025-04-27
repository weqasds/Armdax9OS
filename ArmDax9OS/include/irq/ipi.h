#pragma once
#include <irq/irq.h>
#include <common/types.h>
#include <arch/arm/ipi.h>
enum ipi_state
{
    IPI_RESCHEDULE = 0, // 请求调度
    IPI_CALL_FUNC,      // 请求远核执行函数
    IPI_TLB_FLUSH,      // 请求远核清除TLB
    IPI_WAKEUP,         // 唤醒从核
    IPI_STOP_CPU,       // 请求关停CPU
    IPI_PRINTK_SYNC,    // 打印同步（可选）
    IPI_CUSTOM,         // 自定义
    IPI_NR              // 向量数
};

struct ipi_data
{
    /* data */
    volatile u16 start;
    volatile u16 finish;
    enum ipi_state vector;
    u64 args[7];
};
extern struct ipi_data ipi_datas[];
//架构处理函数
void arch_send_ipi(u32 cpu, u32 ipi);
void arch_handle_ipi(u32 ipi);

//初始化处理期间通信数据
void init_ipi_data(void);


