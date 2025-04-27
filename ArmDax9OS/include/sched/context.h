#pragma once
#include <arch/arm/sched/arch_sched.h>
#include <common/types.h>
#include <sched/sched.h>
#include <mm/vmspace.h>

/// @brief 线程调度上下文
typedef struct thread_sched_context
{
    u32 budget; // 时间片
    u32 prio;   // 优先级
} thread_shced_ctx_t;

/// @brief 进程调度上下文
typedef struct process_sched_context {
    u32 prio;           // 进程优先级（通常是内部线程优先级的上限或平均）
    u32 active_threads; // 当前活跃（runnable）的线程数量
    u32 cpu_affinity;   // 首选CPU（可选，如果支持CPU亲和性）
} process_sched_ctx_t;




struct process_context
{
    /// @brief 进程寄存器上下文结构
    arch_process_context_t p_context;
    /// @brief 进程调度上下文
    process_sched_ctx_t *psc;
    /// @brief pid
    pid_t pid;
    /// @brief 地址空间id
    struct vmspace space;
};

struct thread_context
{
    /// @brief 寄存器上下文结构
    arch_thread_context_t ec;
    /// @brief fpu状态
    void *fpu_state;
    /* @brief tls 寄存器组 
        一般用TPIDR_EL0寄存器
    */
    unsigned long tls_base_reg[TLS_REG_NUM];
    /// @brief fpu所有权
    u32 is_fpu_owner;
    /// @brief 线程调度上下文
    process_sched_ctx_t *tsc;
    /// @brief 线程类型
    u32 thread_type;
    /// @brief 线程状态
    u32 thread_state;
    /// @brief 处理器亲和性
    u32 affinity;
    /// @brief cpuid
    u32 cpuid;
    volatile u32 kernel_stack_state;
    volatile u32 thread_exit_state;

    /* data */
} ALIGN(CACHELINE_SZ);