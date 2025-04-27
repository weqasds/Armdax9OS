#pragma once
#include <common/types.h>
#include <arch/arm/machine/registers.h>

#define TLS_REG_NUM 1

/* 寄存器上下文结构(用于上下文保存) */
typedef struct
{
    u64 x[31];         // X0-X30寄存器
    u64 sp;            // 栈指针
    u64 spsr;          // 程序状态寄存器
    u64 elr;           // 异常链接寄存器
} arch_thread_context_t;

/// @brief 进程特有寄存器上下文结构
typedef struct process_exec_context 
{
    u64 ttbr0_el1;     // 页表基址寄存器 (TTBR0_EL1)，用于用户态
    u64 ttbr1_el1;     // 页表基址寄存器 (TTBR1_EL1)，用于内核态（可选，看你设计）

    u64 tpidr_el0;     // 用户态线程本地存储指针 (TPIDR_EL0)
    u64 tpidrro_el0;   // 用户态只读线程本地存储指针 (TPIDRRO_EL0)

    u64 context_id;    // 上下文ID（通常对应ASID，Address Space ID）
} arch_process_context_t;
