#include <arch/arm/machine/pmu.h>
#include <common/types.h>
#include <common/macro.h>

// PMU初始化
void pmu_init(void) {
    u64 pmcr;
    
    // 读取当前PMCR配置
    asm volatile("mrs %0, " PMCR_EL0 : "=r"(pmcr));
    
    // 设置基本参数：启用计数器、64位周期计数器、时钟分频（24MHz->12MHz）
    pmcr |= PMCR_EL0_E | PMCR_EL0_LC | PMCR_EL0_D;
    
    // 复位所有计数器
    pmcr |= PMCR_EL0_P | PMCR_EL0_C;

    asm volatile("msr " PMCR_EL0 ", %0" ::"r"(pmcr));

    // 配置用户态访问权限 (允许EL0访问)
    asm volatile("msr " PMUSERENR_EL0 ", %0" ::"r"((PMUSERENR_EL0_EN | PMUSERENR_EL0_CR | PMUSERENR_EL0_SW | PMUSERENR_EL0_SW)));

    // 使能周期计数器
    asm volatile("msr " PMCNTENSET_EL0 ", %0" :: "r"(PMCNTENSET_EL0_C));
}

// 启用指定计数器
void pmu_enable_counter(u8 counter) {
    asm volatile("msr " PMCNTENSET_EL0 ", %0" :: "r" (1UL << counter));
}

// 禁用指定计数器
void pmu_disable_counter(u8 counter) {
    asm volatile("msr " PMCNTENCLR_EL0 ", %0" :: "r" (1UL << counter));
}

// 设置计数器事件类型
void pmu_set_event(u8 counter, u32 event) {
    // 选择计数器
    asm volatile("msr " PMSELR_EL0 ", %0" :: "r" (counter & 0x1F));
    // 设置事件类型
    asm volatile("msr " PMXEVTYPER_EL0 ", %0" :: "r" (event));
}
