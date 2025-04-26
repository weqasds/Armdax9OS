#include <common/asm.h>
#include "irq_entry.h"

ASM_FUNC_BEGIN()
ASM_FUNC_END()

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
ASM_FUNC_BEGIN()
ASM_FUNC_END()