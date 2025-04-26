

#include <common/ash.h>
#include <common/vars.h> 

ASM_FUNC_BEGIN(flush_dcache_area)
ASM_FUNC_END(flush_dcache_area)

ASM_FUNC_BEGIN(put32)
    str w1, [x0]
    ret
ASM_FUNC_END(put32)

ASM_FUNC_BEGIN(get32)
    ldr w0, [x0]
    ret
ASM_FUNC_END(get32)

ASM_FUNC_BEGIN(put_64)
    str x1, [x0]
    ret
ASM_FUNC_END(put_64)

ASM_FUNC_BEGIN(get_64)
    ldr x0, [x0]
    ret
ASM_FUNC_END(get_64)