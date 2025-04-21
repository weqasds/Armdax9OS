#pragma once 
#include <common/types.h>
#include <arch/arm/plat/raspi3/machine.h>
#include <common/vars.h>

enum cpu_state{
    cpu_hang = 0,
    cpu_run  = 1,
    cpu_idle = 2,
};
extern volatile char cpu_status[PLAT_CPU_NUM];
