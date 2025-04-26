#pragma once
#include <common/types.h>
#include <common/macro.h>

// PMU寄存器定义
#define PMCR_EL0          "pmcr_el0"
#define PMCNTENSET_EL0    "pmcntenset_el0"
#define PMCNTENCLR_EL0    "pmcntenclr_el0"
#define PMCCNTR_EL0       "pmccntr_el0"
#define PMCCFILTR_EL0     "pmccfiltr_el0"
#define PMSELR_EL0        "pmselr_el0"
#define PMXEVTYPER_EL0    "pmxevtyper_el0"
#define PMXEVCNTR_EL0     "pmxevcntr_el0"
#define PMUSERENR_EL0     "pmuserenr_el0"

// PMCR寄存器位定义
#define PMCR_EL0_MASK         (0x3f)  // 掩码
#define PMCR_EL0_E            BIT(0)  // 启用计数器 0b1
#define PMCR_EL0_P            BIT(1)  // 事件计数器复位 0b10
#define PMCR_EL0_C            BIT(2)  // 周期计数器复位 0b100
#define PMCR_EL0_D            BIT(3)  // 时钟分频器 0b1000
#define PMCR_EL0_X            BIT(4)  // 启用事件流导出 0b10000
#define PMCR_EL0_DP           BIT(5)  // 禁止事件计数时自动禁用周期计数器 0b100000
#define PMCR_EL0_LC           BIT(6)  // 长周期计数器 0b1000000

// 性能监控事件类型
#define CPU_CYCLES_EVENT          0x11
#define INST_RETIRED_EVENT        0x08
#define L1D_CACHE_REFILL_EVENT    0x03
#define MEM_ACCESS_EVENT          0x13

//用户访问权限宏
#define PMUSERENR_EL0_EN     BIT(0)  // 允许EL0(用户态)访问性能监控寄存器
#define PMUSERENR_EL0_SW     BIT(1)   // 允许用户态写SW_INCR寄存器
#define PMUSERENR_EL0_CR     BIT(2)   // 允许用户态读取周期计数器
#define PMUSERENR_EL0_ER     BIT(3)   // 允许用户态读取事件计数器
#define PMCNTENSET_EL0_C     BIT(31)  // 启用周期计数器 (PMCCNTR_EL0)

void pmu_init(void);
void pmu_enable_counter(u8 counter);
void pmu_disable_counter(u8 counter);
void pmu_set_event(u8 counter, u32 event);  // 添加缺失的分号

static inline u64 pmu_read_real_cycle(void) {
    u64 value;
    asm volatile("mrs %0, " PMCCNTR_EL0 : "=r"(value));
    return value;
}
//重置计数器
static inline void pmu_reset_counters(void) {
    asm volatile("msr " PMCR_EL0 ", %0" :: "r" (PMCR_EL0_P | PMCR_EL0_C));
}
