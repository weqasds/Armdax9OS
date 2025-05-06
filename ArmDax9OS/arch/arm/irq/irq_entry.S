#include <common/asm.h>
#include <arch/arm/machine/registers.h>
#include "irq_entry.h"
#include <arch/arm/machine/esr.h>

.extern syscall_table
.extern lock_kernel
.extern unlock_kernel
.extern do_page_fault
.extern handle_entry_c

.macro exception_enter
    // 保存通用寄存器
    stp x0, x1, [sp, #-16]!
    stp x2, x3, [sp, #-16]!
    stp x4, x5, [sp, #-16]!
    stp x6, x7, [sp, #-16]!
    stp x8, x9, [sp, #-16]!
    stp x10, x11, [sp, #-16]!
    stp x12, x13, [sp, #-16]!
    stp x14, x15, [sp, #-16]!
    stp x16, x17, [sp, #-16]!
    stp x18, x19, [sp, #-16]!
    stp x20, x21, [sp, #-16]!
    stp x22, x23, [sp, #-16]!
    stp x24, x25, [sp, #-16]!
    stp x26, x27, [sp, #-16]!
    stp x28, x29, [sp, #-16]!
    stp x30, xzr, [sp, #-16]!

    // 保存特殊寄存器
    mrs x0, sp_el0
    mrs x1, elr_el1
    mrs x2, spsr_el1
    stp x0, x1, [sp, #-16]!
    str x2, [sp, #-8]!
.endm

.macro exception_exit
    // 恢复特殊寄存器
    ldr x2, [sp], #8
    ldp x0, x1, [sp], #16
    msr sp_el0, x0
    msr elr_el1, x1
    msr spsr_el1, x2

    // 恢复通用寄存器
    ldp x30, xzr, [sp], #16
    ldp x28, x29, [sp], #16
    ldp x26, x27, [sp], #16
    ldp x24, x25, [sp], #16
    ldp x22, x23, [sp], #16
    ldp x20, x21, [sp], #16
    ldp x18, x19, [sp], #16
    ldp x16, x17, [sp], #16
    ldp x14, x15, [sp], #16
    ldp x12, x13, [sp], #16
    ldp x10, x11, [sp], #16
    ldp x8, x9, [sp], #16
    ldp x6, x7, [sp], #16
    ldp x4, x5, [sp], #16
    ldp x2, x3, [sp], #16
    ldp x0, x1, [sp], #16

    eret
.endm

// 异常向量表
.align 11
EXPORT(el1_vector)
    /* Each entry should be 0x80 aligned */
    .align 7
    // EL1t
    b sync_el1t_handler
    .align 7
    b irq_el1t_handler
    .align 7
    b fiq_el1t_handler
    .align 7
    b error_el1t_handler
    .align 7

    // EL1h
    b sync_el1h_handler
    .align 7
    b irq_el1h_handler
    .align 7
    b fiq_el1h_handler
    .align 7
    b error_el1h_handler
    .align 7

    // EL0_64
    b sync_el0_64_handler
    .align 7
    b irq_el0_64_handler
    .align 7
    b fiq_el0_64_handler
    .align 7
    b error_el0_64_handler
    .align 7

    // EL0_32
    b sync_el0_32_handler
    .align 7
    b irq_el0_32_handler
    .align 7
    b fiq_el0_32_handler
    .align 7
    b error_el0_32_handler

// 系统调用处理
el0_syscall:
    exception_enter
    // 检查是否为SVC指令触发的系统调用
    mrs x1, esr_el1
    lsr x0, x1, #ESR_EL1_EC_SHIFT
    and x0, x0, #EC_MASK
    cmp x0, #ESR_EL1_EC_SVC_64
    b.ne 1f
    
    // 系统调用处理
    bl lock_kernel
    // 从x8获取系统调用号
    mov x0, x8
    // 检查系统调用号是否有效
    cmp x0, #NR_SYSCALL
    b.hs 2f
    // 调用对应的系统调用处理函数
    ldr x1, =syscall_table
    ldr x1, [x1, x0, lsl #3]
    blr x1
    b 3f
    
2:  // 无效系统调用号处理
    mov x0, #-1
    
3:  // 系统调用返回
    bl unlock_kernel
    b exception_exit
    
1:  // 非系统调用同步异常处理
    mov x0, #SYNC_EL0_64
    mrs x2, elr_el1
    bl handle_entry_c
    exception_exit

// 需要检查系统调用的异常处理程序
sync_el0_64_handler:
    b el0_syscall

sync_el0_32_handler:
    exception_enter
    // 检查是否为SVC指令触发的系统调用
    mrs x1, esr_el1
    lsr x0, x1, #ESR_EL1_EC_SHIFT
    and x0, x0, #EC_MASK
    cmp x0, #ESR_EL1_EC_SVC_32
    b.eq el0_syscall
    
    // 非系统调用同步异常处理
    mov x0, #SYNC_EL0_32
    mrs x2, elr_el1
    bl handle_entry_c
    exception_exit

// 其他异常处理程序保持不变
irq_el0_64_handler:
    exception_enter
    mov x0, #IRQ_EL0_64
    mrs x1, esr_el1
    mrs x2, elr_el1
    bl handle_entry_c
    exception_exit

fiq_el0_64_handler:
    exception_enter
    mov x0, #FIQ_EL0_64
    mrs x1, esr_el1
    mrs x2, elr_el1
    bl handle_entry_c
    exception_exit

error_el0_64_handler:
    exception_enter
    mov x0, #ERROR_EL0_64
    mrs x1, esr_el1
    mrs x2, elr_el1
    bl handle_entry_c
    exception_exit

irq_el0_32_handler:
    exception_enter
    mov x0, #IRQ_EL0_32
    mrs x1, esr_el1
    mrs x2, elr_el1
    bl handle_entry_c
    exception_exit

fiq_el0_32_handler:
    exception_enter
    mov x0, #FIQ_EL0_32
    mrs x1, esr_el1
    mrs x2, elr_el1
    bl handle_entry_c
    exception_exit

error_el0_32_handler:
    exception_enter
    mov x0, #ERROR_EL0_32
    mrs x1, esr_el1
    mrs x2, elr_el1
    bl handle_entry_c
    exception_exit

sync_el1t_handler:
    exception_enter
    mov x0, #SYNC_EL1t
    mrs x1, esr_el1
    mrs x2, elr_el1
    bl handle_entry_c
    exception_exit

irq_el1t_handler:
    exception_enter
    mov x0, #IRQ_EL1t
    mrs x1, esr_el1
    mrs x2, elr_el1
    bl handle_entry_c
    exception_exit

fiq_el1t_handler:
    exception_enter
    mov x0, #FIQ_EL1t
    mrs x1, esr_el1
    mrs x2, elr_el1
    bl handle_entry_c
    exception_exit

error_el1t_handler:
    exception_enter
    mov x0, #ERROR_EL1t
    mrs x1, esr_el1
    mrs x2, elr_el1
    bl handle_entry_c
    exception_exit

sync_el1h_handler:
    exception_enter
    mov x0, #SYNC_EL1h
    mrs x1, esr_el1
    mrs x2, elr_el1
    bl handle_entry_c
    exception_exit

irq_el1h_handler:
    exception_enter
    mov x0, #IRQ_EL1h
    mrs x1, esr_el1
    mrs x2, elr_el1
    bl handle_entry_c
    exception_exit

fiq_el1h_handler:
    exception_enter
    mov x0, #FIQ_EL1h
    mrs x1, esr_el1
    mrs x2, elr_el1
    bl handle_entry_c
    exception_exit

error_el1h_handler:
    exception_enter
    mov x0, #ERROR_EL1h
    mrs x1, esr_el1
    mrs x2, elr_el1
    bl handle_entry_c
    exception_exit
