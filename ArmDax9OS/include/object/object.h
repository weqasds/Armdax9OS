#pragma once
#include <arch/arm/plat/raspi3/machine.h>
#include <common/types.h>
#include <mm/vmspace.h>
#include <sched/sched.h>
#include <sched/context.h>
#include <arch/arm/machine/smp.h>
#include <common/lock.h>
extern struct process *current_process[PLAT_CPU_NUM];
extern struct process idle_process[PLAT_CPU_NUM];
#define current_process (current_process[get_gpu_id()])
#define DEFAULT_KERNEL_STACK_SZ (0x1000)


struct process
{
    /// @brief 进程上下文结构
    struct process_context *pct;
    /// @brief 线程链表
    struct list_head thread_list;
    /// @brief 线程数量
    u32 thread_cnt;
    struct spin_lock process_lock;
    /// @brief 进程地址空间
    struct vmspace space;
    /// @brief 进程id
    pid_t pid;
    /// @brief 进程主线程id
    s64 tpid;
    

    
};

struct thread
{
    /// @brief 同一进程的所有线程
    struct list_head node;
    /// @brief 就绪队列中的线程
    struct list_head ready_queue_node;
    /// @brief 信号量等待队列线程
    struct list_head sem_queue_node;
    /// @brief 线程上下文结构
    struct thread_context *thread_ctx;
    /// @brief 所属父进程
    struct process *parent_process;
    /// @brief 上一个运行线程
    struct thread *prev_thread;
};

void create_root_process(void);
void switch_process_vmspace_to(struct process *); 


