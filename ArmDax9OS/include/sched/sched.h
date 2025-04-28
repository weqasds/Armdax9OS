#pragma once
#include <arch/arm/plat/raspi3/machine.h>
#include <arch/arm/sched/arch_sched.h>
#include <arch/arm/sync.h>
#include <common/types.h>
#include <lib/printf.h>

//声明线程与进程结构
struct thread;
struct process;

/// @brief 进程类型
enum procee_type
{
    PROCESS_TYPE_USER,      // 普通用户进程
    PROCESS_TYPE_KERNEL,    // 内核服务进程
    PROCESS_TYPE_REALTIME,  // 实时进程（高优先级）
    PROCESS_TYPE_DAEMON,    // 守护进程（长期运行）
    PROCESS_TYPE_SYSTEM,    // 系统关键进程
};

/// @brief 线程类型
enum thread_type
{
    /*
     * Kernel-level threads won't swap gs/fs
     */
    THREAD_TYPE_IDLE = 0,   /* IDLE thread dose not have stack, pause cpu */
    THREAD_TYPE_KERNEL = 1, /* KERNEL thread has stack */

    /*
     * User-level threads
     * Should be larger than TYPE_KERNEL!
     */
    THREAD_TYPE_USER = 2,
    THREAD_TYPE_SHADOW = 3, /* SHADOW thread is used to achieve migrate IPC */
    /* Use as the IPC register callback threads (for recycling) */
    THREAD_TYPE_TESTS = 4 /* TESTS thread is used by kernel tests */
};

/// @brief 进程状态
enum process_state
{
    PROCESS_STATE_INIT = 0,  // 初始化
    PROCESS_STATE_READY,     // 就绪（可调度）
    PROCESS_STATE_RUNNING,   // 正在执行
    PROCESS_STATE_BLOCKED,   // 阻塞（等待资源）
    PROCESS_STATE_SUSPENDED, // 被挂起（如调试）
    PROCESS_STATE_ZOMBIE,    // 已终止待回收
    PROCESS_STATE_DEAD,      // 完全终止
};

/// @brief 线程状态
enum thread_state
{
    THREAD_STATE_INIT = 0,
    THREAD_STATE_READY,
    THREAD_STATE_INTER, /* Intermediate stat used by sched (only for debug) */
    THREAD_STATE_RUNNING,
    THREAD_STATE_EXIT,    /* Only for debug use */
    THREAD_STATE_WAITING, /* Waiting IPC or etc */
};

/// @brief 进程调度操作
struct process_sched_ops
{
    /// @brief 进程初始化
    u32 (*process_init)(void);
    /// @brief 进程入队
    u32 (*process_enqueue)(struct process *p);
    /// @brief 进程出队
    u32 (*process_dequeue)(struct process *p);
    /// @brief 选择进程
    u32 (*process_pick_next)(void);
    /// @brief 周期性调度进程
    u32 (*process_process_tick)(void);
};

/// @brief 线程调度操作
struct thread_sched_ops
{
    u32 (*thread_init)(void);
    u32 (*yield)(struct thread *t);
    u32 (*wakeup)(struct thread *t);
    u32 (*priority_set)(struct thread *t, u32 prio);
};


/// @brief rr调度策略
extern struct process_sched_ops rr_process;
extern struct thread_sched_ops rr_thread;
/// @brief 优先级调度策略
extern struct process_sched_ops prio_process;
extern struct thread_sched_ops prio_thread;
/// @brief 基于优先级的fifo调度策略
extern struct process_shced_ops prio_fifo_process;
extern struct thread_sched_ops prio_fifo_thread;
