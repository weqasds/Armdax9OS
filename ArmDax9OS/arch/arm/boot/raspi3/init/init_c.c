
#include <common/types.h>
#include <arch/arm/machine/registers.h>
#include "const.h"
#include "init.h"

char boot_cpu_stack[PLAT_CPU_NUM][INIT_STACK_SIZE] ALIGN(16);

#define NOT_BSS (0xBEEFUL)
volatile u64 clear_bss_flag = NOT_BSS;
long secondary_boot_flag[PLAT_CPU_NUM] = {NOT_BSS};
// in peripherals/uart.c
void early_uart_init(void);
void early_uart_puts(char *);

static inline void enable_other_cores(void)
{
    u64 *addr;

    /*
     * Set the entry address for non-primary cores.
     * 0xe0, 0xe8, 0xf0 are fixed in the firmware (armstub8.bin).
     */
    addr = (u64 *)0xe0;
    *addr = TEXT_OFFSET;
    addr = (u64 *)0xe8;
    *addr = TEXT_OFFSET;
    addr = (u64 *)0xf0;
    *addr = TEXT_OFFSET;
    asm volatile("sev");
}

/// @brief 清除bss段
/// @param
static void clear_bss(void)
{
    extern u64 bss_start_addr, bss_end_addr;
    volatile u64 p;

    // 确保clear_bss_flag在bss段
    if (clear_bss_flag != NOT_BSS)
    {
        return;
    }
    bss_start_addr=(u64)&_bss_start;
    bss_end_addr=(u64)&_bss_end;

    // 清除bss段
    for (p = bss_start_addr; p < bss_end_addr; p++)
    {
        *(char *)p = 0;
    }

    // 标记已清除
    clear_bss_flag = 0;
}

/// @brief 初始化
/// @param
void init_c(void)
{

    // clear bss
    clear_bss();
    early_uart_init();
    early_uart_puts("boot: init_c\n");
    enable_other_cores();

    early_uart_puts("hello world\n");
    early_uart_puts("enable others cores");
    init_boot_pt();
    early_uart_puts("init page table");
    el1_mmu_activate();
    early_uart_puts("el1_mmu_activate");
    // enter kernel
    start_kernel(secondary_boot_flag);
}

/// @brief 从核初始化代码
/// @param
void secondary_init_c(u32 cpu_id)
{
    el1_mmu_activate();
    secondary_cpu_entry(cpu_id);
}
