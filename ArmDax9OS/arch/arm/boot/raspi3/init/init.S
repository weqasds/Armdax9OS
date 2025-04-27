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
    

    	
/* Wait for bss clear */
wait_for_bss_clear:
	adr	x0, clear_bss_flag
	ldr	x1, [x0]
	cmp     x1, #0
	bne	wait_for_bss_clear //循环等待直到clear_bss_flag被改

	/* Turn to el1 from other exception levels. */
	bl 	switch_elx_to_el1

	/* Prepare stack pointer and jump to C. */
	mov	x1, #INIT_STACK_SIZE
	mul	x1, x8, x1
	ldr 	x0, =boot_cpu_stack
	add	x0, x0, x1
	add	x0, x0, #INIT_STACK_SIZE
	mov	sp, x0

    // 从核处理流程
secondary_core:
    mov     x1,  #8
    mul     x2,  x8,  x1
    // 等待主核设置secondary_boot_flag
    ldr     x1, =secondary_boot_flag
    add     x1,  x1,  x2
    ldr     x3, [x1]
    cbz     x3, secondary_core  // 如果标志为0则继续等待
    //内存屏障，保证顺序
    wfe                         // 等待事件
    
    ldr     x0, =secondary_init_c
    cbz     x0, secondary_core  // 检查secondary_init_c是否已设置
    br      x0                  // 跳转到secondary_init_c
    
    // 主核处理流程
primary_core:
    // 1. 切换到EL1异常等级
    bl      switch_elx_to_el1

    // 2. 初始化主核栈指针
    adr     x0, boot_cpu_stack
    add     x0, x0, #INIT_STACK_SIZE   // 栈大小为8KB
    mov     sp, x0

    //跳转到init_c 完成bss段清零 唤醒从核 初始化页表 启用el1_mmu 进入内核
    bl init_c

    b .
ASM_FUNC_END(_start)
