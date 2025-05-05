#include <common/asm.h>
#include "irq_entry.h"

ASM_FUNC_BEGIN(set_exception_vector)
	adr	x0, el1_vector
	msr	vbar_el1, x0
	ret
ASM_FUNC_END(set_exception_vector)

/* 
 * DAIF 4 bits
 * D: debug exceptions
 * A: asynchronous aborts SError
 * I: IRQs
 * F: FIQs
 */
ASM_FUNC_BEGIN(enable_irq)
	msr	daifclr, #2
	ret
ASM_FUNC_END(enable_irq)

ASM_FUNC_BEGIN(disable_irq)
    msr     daifset, #2
    ret
ASM_FUNC_END(disable_irq)

ASM_FUNC_BEGIN(save_irq)
    mrs x0, daif    // 读取完整的DAIF寄存器值
    ret             // 返回全部4位状态
ASM_FUNC_END(save_irq)

ASM_FUNC_BEGIN(restore_irq)
    msr daif, x0    // 恢复完整的DAIF寄存器值
    ret             // 返回
ASM_FUNC_END(restore_irq)
