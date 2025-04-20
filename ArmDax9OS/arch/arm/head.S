#include <common/asm.h>
#include <common/vars.h>

.section ".text.boot"
ASM_FUNC_BEGIN(_start)
    // 设置栈指针(使用KBASE作为基地址)
    ldr x0, =KBASE
    mov sp, x0
    
    // 设置栈大小并保持16字节对齐
    ldr x1, =KERNEL_STACK_SIZE
    add sp, sp, x1
    and sp, sp, #~STACK_ALIGNMENT
    
    // 跳转到main函数
    bl main
    
    // 主核进入空闲循环
1:  wfe
    b 1b
ASM_FUNC_END(_start)

// 从核启动入口
ASM_FUNC_BEGIN(secondary_entry)
    // 设置从核栈(比主核栈小一些)
    ldr x0, =KBASE
    ldr x1, =IDLE_STACK_SIZE
    add sp, x0, x1
    and sp, sp, #~STACK_ALIGNMENT
    
    // 跳转到secondary_start函数
    bl secondary_start
    
    // 从核进入空闲循环
1:  wfe
    b 1b
ASM_FUNC_END(secondary_entry)
