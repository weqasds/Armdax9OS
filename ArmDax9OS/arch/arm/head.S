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
    
    bl flush_tlb_all //清除tlb

    mov x3, #0
    // 跳转到main函数
    msr TPIDR_EL1, x3
    ldr x0, [sp], #8
    bl main
    
ASM_FUNC_END(start_kernel)

// 从核启动
ASM_FUNC_BEGIN(secondary_cpu_entry)
    // 1. 保存CPU ID到TPIDR_EL1（x0来自secondary_init_c参数）
    msr TPIDR_EL1, x0
    isb
    
    // 2. 设置从核栈（与init.S第40-45行完全一致）
    mov x1, #INIT_STACK_SIZE      // 8KB栈大小
    mul x1, x0, x1                // cpu_id * stack_size
    ldr x2, =boot_cpu_stack       // 使用相同的栈数组
    add x2, x2, x1                // 计算当前核心栈基址
    add sp, x2, #INIT_STACK_SIZE  // 设置栈顶指针
    
    // 3. 设置异常向量表（与主核start_kernel保持一致）
    ldr x2, =vectors
    msr vbar_el1, x2
    isb
    
    // 4. 初始化MMU（复用主核页表配置）
    bl el1_mmu_activate
    
    // 5. 跳转到内核入口（参数传递与主核对齐）
    mov x0, #0                   // 清零参数保持与主核一致
    bl start_kernel
    
    // 6. 安全停机循环（使用WFI指令节能）
1:  wfi
    b 1b
ASM_FUNC_END(secondary_cpu_entry)
