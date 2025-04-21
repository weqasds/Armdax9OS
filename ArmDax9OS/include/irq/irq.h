#pragma once
#include <common/types.h>
#include <irq_num.h>

#define HANDLE_KERNEL 0
#define HANDLE_USER   1
//中断类型向量
extern u8 irq_handle_type[MAX_IRQ_NUM];

/* 中断控制器接口 */
void irq_ctrl_init(void);
void irq_ctrl_enable(u32 irq);
void irq_ctrl_disable(u32 irq);
void irq_ctrl_clear_pending(u32 irq);

/* 中断处理接口 */
typedef void (*irq_handler_t)(u32 irq, void *data);
int request_irq(u32 irq, irq_handler_t handler, void *data);
void free_irq(u32 irq);
void set_irq_priority(u32 irq, u32 priority);
void set_irq_trigger(u32 irq, u32 trigger_type);

/* 中断状态 */
u32 get_pending_irq(void);
bool_ is_irq_enabled(u32 irq);  /* 返回0表示禁用，1表示启用 */
bool_ is_irq_pending(u32 irq);  /* 返回0表示无挂起，1表示挂起 */

/* 架构特定接口 (aarch64) */
void arch_irq_enable(void);
void arch_irq_disable(void);
void arch_irq_save_flags(u64 *flags);
void arch_irq_restore_flags(u64 flags);

/* 平台特定接口 (Raspberry Pi 3) */
void plat_irq_init(void);
void plat_irq_enable(u32 irq);
void plat_irq_disable(u32 irq);
