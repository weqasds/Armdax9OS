#pragma once 
#include <common/types.h>
#include <arch/arm/plat/raspi3/machine.h>
#include <common/vars.h>
enum {
    IPI_RESCHEDULE = 0,      // 请求调度
    IPI_CALL_FUNC,           // 请求远核执行函数
    IPI_TLB_FLUSH,           // 请求远核清除TLB
    IPI_STOP_CPU,            // 请求关停CPU
    IPI_PRINTK_SYNC,         // 打印同步（可选）
    IPI_CUSTOM,              // 自定义
    IPI_NR                   // 向量数
};
