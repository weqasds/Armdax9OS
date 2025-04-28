#pragma once
#include <arch/arm/plat/raspi3/machine.h>
#include <common/types.h>
#include <mm/vmspace.h>
#include <sched/sched.h>
#include <sched/context.h>
struct process
{
    /// @brief 进程上下文结构
    struct process_context *pct;
    /// @brief 线程链表
    struct list_head thread_list;
    /// @brief 地址空间id
    struct vmspace space;
};

struct thread
{
    /// @brief 线程上下文结构
    struct thread_context *tct;
    /// @brief 所属父进程
    struct process *parent_process;
    /// @brief 紧邻的上一个线程
    struct thread *prev_thread;
    /// @brief 紧邻的下一个线程
    struct thread *next_thread;
};


extern struct process *current_process[PLAT_CPU_NUM];
