#pragma once
#include <common/types.h>
#include <irq_num.h>
#include <arch/arm/machine/smp.h>
#include <common/types.h>  // 确保包含u32/u64类型定义

// 中断处理函数类型定义
typedef void (*irq_handler_t)(int irq, void *regs, void *arg);

/* 架构相关中断接口 (ARMv8a) */
// 设置异常向量表基地址
void set_exception_vector_base(u64 base_addr);

// 中断使能/禁用
void arch_irq_enable(void);
void arch_irq_disable(void);
u32 arch_irq_save(void);
void arch_irq_restore(u32 flags);

// 上下文保存恢复
struct pt_regs;
void save_processor_state(struct pt_regs *regs);
void restore_processor_state(struct pt_regs *regs);

/* 平台相关接口 (BCM2837) */
// 中断控制器初始化
void plat_irq_controller_init(void);

// 外设中断配置
void plat_route_irq(u32 irq_num, u32 cpu_mask);
void plat_enable_irq(u32 irq_num);
void plat_disable_irq(u32 irq_num);

// 本地定时器中断
void plat_local_timer_init(u32 freq);
void plat_local_timer_handler(void);

/* SMP相关中断接口 */
// 核间中断处理
void smp_ipi_handler(int ipi_num, struct pt_regs *regs);

// 多核中断初始化
void smp_irq_init_per_cpu(void);

// 中断负载均衡
void irq_set_affinity(u32 irq_num, u32 cpu_mask);

/* 进程/线程调度相关 */
// 中断上下文切换
void schedule_in_irq(struct pt_regs *regs);

// 中断处理标志
#define IRQ_HANDLED    0
#define IRQ_UNHANDLED  1

// 中断类型向量
#define HANDLE_KERNEL 0
#define HANDLE_USER   1
extern u8 irq_handle_type[MAX_IRQ_NUM];

// 中断注册接口
int request_irq(u32 irq_num, irq_handler_t handler, 
               u32 flags, const char *name, void *arg);
void free_irq(u32 irq_num);

// IPI核间中断接口
void send_ipi(u32 cpu_mask, u32 ipi_type);
