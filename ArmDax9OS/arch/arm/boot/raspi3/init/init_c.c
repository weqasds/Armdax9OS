
#include <common/types.h>
#include <arch/arm/machine/registers.h>
#include "const.h"
#include "init.h"

char secondary_boot_flag[PLAT_CPU_NUM][INIT_STACK_SIZE] ALIGN(16);

#define NOT_BSS (0xBEEFUL)
volatile u64 clear_bss_flag = {NOT_BSS};
//in peripherals/uart.c
void early_uart_init(void);
void early_uart_puts(char*);



/// @brief 清除bss段
/// @param  
static void clear_bss(void){
    extern char __bss_start[], __bss_end[];
    volatile u64 *p;
    
    // 确保clear_bss_flag在bss段
    if (clear_bss_flag != NOT_BSS) {
        return;
    }

    // 清除bss段
    for (p = (volatile u64 *)__bss_start; p < (volatile u64 *)__bss_end; p++) {
        *p = 0;
    }
    
    // 标记已清除
    clear_bss_flag = 0;
}

/// @brief 初始化
/// @param  
void init_c(void){
    
    //
    clear_bss();
    early_uart_init();
    early_uart_puts("boot: init_c\n");
    early_uart_puts("boot: init_c\n");
    early_uart_puts("hello world\n");
}

/// @brief 从核初始化代码
/// @param  
void secondary_init_c(void){}
