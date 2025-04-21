#include <common/asm.h>
#include <common/vars.h>

ASM_FUNC_BEGIN(start_kernel)
    // 设置栈指针(使用KBASE作为基地址)
    ldr x2, =kernel_stack
    add x2, x2, KERNEL_STACK_SIZE
    mov sp, x2
    
    str x0, [sp,#-8]!
    mov x2, #0

    msr ttbr0_el1, x2
    isb
    
    bl flush_tlb_all

    mov x3, #0
    // 跳转到main函数
    msr TPIDR_EL1, x3
    ldr x0, [sp], #8
    bl main
    
ASM_FUNC_END(start_kernel)

// 从核启动入口
ASM_FUNC_BEGIN(secondary_entry)
    // 设置从核栈(比主核栈小一些)
    msr TPIDR_EL1, x0
    mov x1, #KERNEL_STACK_SIZE
    add sp, x0, x1
    and sp, sp, #~STACK_ALIGNMENT
    
    // 跳转到secondary_start函数
    bl secondary_start
    
    // 从核进入空闲循环
1:  wfe
    b 1b
ASM_FUNC_END(secondary_entry)
