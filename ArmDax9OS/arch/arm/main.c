#include<common/types.h>
#include<common/lock.h>
#include<common/kprintf.h>
#include<common/types.h> 
#include <arch/arm/plat/raspi3/machine.h>
#include<common/vars.h>
char kernel_stack[PLAT_CPU_NUM][KERNEL_STACK_SIZE];


/// @brief 内核启动入口
/// @param bootflag 
void main(paddr_t bootflag){}

/// @brief arm从核启动
/// @param  
void secndary_start(void){}