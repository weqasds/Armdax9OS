#pragma once

extern void el1_mmu_activate(void);
extern void init_boot_pt(void);

//in arch/arm/head.S
extern void start_kernel(void* boot_flag);
extern void secondary_cpu_entry(int cpu_id);
//bss段开始位置
extern char _bss_start;
//bss段结束位置
extern char _bss_end;

//same as arch/arm/plat/machine.h
#define PLAT_CPU_NUM 4 
//same as common/macro.h
#define ALIGN(n) __attribute__((__aligned__(n)))