#include <asm.h>

/*
 * include/mm/mm.h
 * void *memmove(void *dst, const void *src, size_t size)
 * aarch64 general purpose register optimized memmove (handles overlapping regions)
 */
ASM_FUNC_BEGIN(memmove)
    mov     x3, x0          // Save original dst
    cmp     x0, x1
    b.eq    .Ldone          // dst == src, nothing to do
    b.hi    .Lbackward_copy // dst > src, need backward copy

    // Forward copy (dst < src)
    // Handle small copies (<= 128 bytes) with simple loop
    cmp     x2, #128
    b.le    .Lsmall_copy

    // Align destination to 16-byte boundary
    neg     x4, x0
    ands    x4, x4, #15
    b.eq    .Laligned_dst

    // Copy unaligned head
    sub     x2, x2, x4
.Lhead_loop:
    ldrb    w5, [x1], #1
    strb    w5, [x0], #1
    subs    x4, x4, #1
    b.ne    .Lhead_loop

.Laligned_dst:
    // Main copy loop with 64-bit registers
    lsr     x4, x2, #6      // x4 = count / 64
    cbz     x4, .Ltail_copy

.Lcopy_loop:
    ldp     x5, x6, [x1], #16
    ldp     x7, x8, [x1], #16
    ldp     x9, x10, [x1], #16
    ldp     x11, x12, [x1], #16
    stp     x5, x6, [x0], #16
    stp     x7, x8, [x0], #16
    stp     x9, x10, [x0], #16
    stp     x11, x12, [x0], #16
    subs    x4, x4, #1
    b.ne    .Lcopy_loop

    // Handle remaining bytes (<128)
    and     x2, x2, #127
.Ltail_copy:
    cbz     x2, .Ldone

.Lsmall_copy:
    ldrb    w5, [x1], #1
    strb    w5, [x0], #1
    subs    x2, x2, #1
    b.ne    .Lsmall_copy
    b       .Ldone

    // Backward copy (dst > src)
.Lbackward_copy:
    add     x0, x0, x2      // Point to end of dst
    add     x1, x1, x2      // Point to end of src

    // Handle small copies (<= 128 bytes) with simple loop
    cmp     x2, #128
    b.le    .Lsmall_copy_back

    // Align destination to 16-byte boundary
    ands    x4, x0, #15
    b.eq    .Laligned_dst_back

    // Copy unaligned tail
    sub     x2, x2, x4
.Ltail_loop_back:
    ldrb    w5, [x1, #-1]!
    strb    w5, [x0, #-1]!
    subs    x4, x4, #1
    b.ne    .Ltail_loop_back

.Laligned_dst_back:
    // Main copy loop with 64-bit registers
    lsr     x4, x2, #6      // x4 = count / 64
    cbz     x4, .Ltail_copy_back

.Lcopy_loop_back:
    ldp     x5, x6, [x1, #-16]!
    ldp     x7, x8, [x1, #-16]!
    ldp     x9, x10, [x1, #-16]!
    ldp     x11, x12, [x1, #-16]!
    stp     x5, x6, [x0, #-16]!
    stp     x7, x8, [x0, #-16]!
    stp     x9, x10, [x0, #-16]!
    stp     x11, x12, [x0, #-16]!
    subs    x4, x4, #1
    b.ne    .Lcopy_loop_back

    // Handle remaining bytes (<128)
    and     x2, x2, #127
.Ltail_copy_back:
    cbz     x2, .Ldone

.Lsmall_copy_back:
    ldrb    w5, [x1, #-1]!
    strb    w5, [x0, #-1]!
    subs    x2, x2, #1
    b.ne    .Lsmall_copy_back

.Ldone:
    mov     x0, x3          // Return original dst
    ret
ASM_FUNC_END(memmove)
