#include "const.h"
#include <include/common/asm.h>
#include <init.h>

.extern switch_elx_to_el1    // 函数定义 - 切换到EL1异常等级

.extern init_c               // 函数定义 - 主核C语言初始化入口

.extern secondary_init_c     // 函数定义 - 从核C语言初始化入口

.extern clear_bss_flag       // 标志 - BSS段清零标志

.extern secondary_boot_flag  // 标志 - 从核启动标志

.extern boot_cpu_stack       // 变量 - 主核栈指针
/*
.global _start;
.type _start, "function"; 
_start:
*/
ASM_FUNC_BEGIN(_start)
    // 检查当前CPU核心ID
    mrs     x0, mpidr_el1
    and     x0, x0, #0xFF       // 获取Aff0字段(核心ID)
    cbz     x0, primary_core    // 如果是核心0则跳转到主核流程
    
    // 从核处理流程
secondary_core:
    // 等待主核设置secondary_boot_flag
    ldr     x1, =secondary_boot_flag
    ldr     w0, [x1]
    cbz     w0, secondary_core  // 如果标志为0则继续等待
    wfe                         // 等待事件
    
    ldr     x0, =secondary_init_c
    cbz     x0, secondary_core  // 检查secondary_init_c是否已设置
    br      x0                  // 跳转到secondary_init_c
    
    // 主核处理流程
primary_core:
    // 1. 初始化主核栈指针
    ldr     x0, =boot_cpu_stack
    add     x0, x0, #INIT_STACK_SIZE   // 栈大小为8KB
    mov     sp, x0
    
    // 2. 切换到EL1异常等级
    bl      switch_elx_to_el1
    
    // 3. 清零BSS段
    ldr     x0, =__bss_start
    ldr     x1, =__bss_end
    sub     x1, x1, x0
    cbz     x1, 4f          // 如果BSS大小为0则跳过
3:
    str     xzr, [x0], #8
    subs    x1, x1, #8
    b.gt    3b
4:
    // 4. 设置从核启动标志
    ldr     x0, =secondary_boot_flag
    mov     w1, #1
    str     w1, [x0]
    dsb     sy
    
    // 5. 跳转到C语言初始化
    ldr     x0, =init_c
    br      x0

ASM_FUNC_END(_start)
