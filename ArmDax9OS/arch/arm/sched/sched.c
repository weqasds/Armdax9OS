#include <arch/arm/sched/arch_sched.h>
#include <sched/context.h>
#include <sched/sched.h>

/// @brief rr调度策略
struct process_sched_ops rr_process;
struct thread_sched_ops rr_thread;
/// @brief 优先级调度策略
struct process_sched_ops prio_process;
struct thread_sched_ops prio_thread;
/// @brief 基于优先级的fifo调度策略
struct process_shced_ops prio_fifo_process;
struct thread_sched_ops prio_fifo_thread;