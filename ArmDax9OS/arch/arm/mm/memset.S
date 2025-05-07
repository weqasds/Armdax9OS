#include <common/asm.h>

/*
 * include/mm/mm.h 
 * void *memset(void *dst, int ch, size_t size)
 * aarch64 general purpose register optimized memset
 */
ASM_FUNC_BEGIN(memset)
    mov     x3, x0          // Save original dst
    and     w1, w1, #0xff   // Clear upper bits of char
    orr     w1, w1, w1, lsl #8  // Duplicate byte to 16-bit
    orr     w1, w1, w1, lsl #16 // Duplicate to 32-bit
    mov     x4, x1          // Prepare 64-bit pattern
    orr     x4, x4, x4, lsl #32

    // Handle small sets (<= 128 bytes) with simple loop
    cmp     x2, #128
    b.le    .Lsmall_set

    // Align destination to 16-byte boundary
    neg     x4, x0
    ands    x4, x4, #15
    b.eq    .Laligned_dst

    // Set unaligned head
    sub     x2, x2, x4
.Lhead_loop:
    strb    w1, [x0], #1
    subs    x4, x4, #1
    b.ne    .Lhead_loop

.Laligned_dst:
    // Main set loop with 64-bit registers
    lsr     x5, x2, #6      // x5 = count / 64
    cbz     x5, .Ltail_set

.Lset_loop:
    stp     x4, x4, [x0], #16
    stp     x4, x4, [x0], #16
    stp     x4, x4, [x0], #16
    stp     x4, x4, [x0], #16
    subs    x5, x5, #1
    b.ne    .Lset_loop

    // Handle remaining bytes (<128)
    and     x2, x2, #127
.Ltail_set:
    cbz     x2, .Ldone

.Lsmall_set:
    strb    w1, [x0], #1
    subs    x2, x2, #1
    b.ne    .Lsmall_set

.Ldone:
    mov     x0, x3          // Return original dst
    ret
ASM_FUNC_END(memset)
