#include <common/asm.h>

/*dcache 宏实现 */
.macro dcache op                     
dsb     sy                       
mrs     x0, clidr_el1            
and     x3, x0, #0x7000000       
lsr     x3, x3, #23              

cbz     x3, finished_\op         
mov     x10, #0                  

loop1_\op:                           
add     x2, x10, x10, lsr #1     
lsr     x1, x0, x2               
and     x1, x1, #7               
cmp     x1, #2                   
b.lt    skip_\op                 

msr     csselr_el1, x10          
isb                              

mrs     x1, ccsidr_el1           
and     x2, x1, #7               
add     x2, x2, #4               
mov     x4, #0x3ff               
and     x4, x4, x1, lsr #3       
clz     w5, w4                   
mov     x7, #0x7fff              
and     x7, x7, x1, lsr #13      

loop2_\op:                           
mov     x9, x4                   

loop3_\op:                           
lsl     x6, x9, x5               
orr     x11, x10, x6             
lsl     x6, x7, x2               
orr     x11, x11, x6             
dc      \op, x11                 
subs    x9, x9, #1               
b.ge    loop3_\op                
subs    x7, x7, #1               
b.ge    loop2_\op                

skip_\op:                            
add     x10, x10, #2             
cmp     x3, x10                  
b.gt    loop1_\op                

finished_\op:                        
mov     x10, #0                  
msr     csselr_el1, x10          
dsb     sy                       
isb                              
.endm  
/*
 * 设置TTBR0_EL1寄存器
 * x0 - 页表物理地址
 */
ASM_FUNC_BEGIN(set_ttbr0_el1)
    msr    ttbr0_el1, x0       // 设置页表基址
    isb                        // 指令同步屏障
    ret
ASM_FUNC_END(set_ttbr0_el1)

/*
 * 获取TTBR0_EL1寄存器值
 * 返回: x0 - 寄存器值
 */
ASM_FUNC_BEGIN(get_ttbr0_el1)
    mrs    x0, ttbr0_el1       // 读取页表基址
    ret
ASM_FUNC_END(get_ttbr0_el1)

/*
 * 刷新数据缓存和指令缓存
 */
ASM_FUNC_BEGIN(flush_idcache)
    dsb    sy                  // 数据同步屏障
    ic     iallu               // 无效化所有指令缓存
    dsb    sy                  // 确保指令缓存无效化完成
    isb                        // 指令同步屏障
    dcache cvau
    dsb sy
    isb
    ret
ASM_FUNC_END(flush_idcache)
