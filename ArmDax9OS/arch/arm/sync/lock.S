#include <common/asm.h>

/* 自旋锁获取 */
ASM_FUNC_BEGIN(arm_spin_lock_acquire)
    mov     x2, #1
1:  ldaxr   w1, [x0]        // 带acquire语义的加载独占
    cbnz    w1, 1b          // 如果锁已被持有，重试
    stxr    w3, w2, [x0]    // 尝试存储独占
    cbnz    w3, 1b          // 如果存储失败，重试
    dmb     ish             // 获取后的内存屏障
    ret
ASM_FUNC_END(arm_spin_lock_acquire)

/* 自旋锁释放 */
ASM_FUNC_BEGIN(arm_spin_lock_release)
    dmb     ish             // 释放前的内存屏障
    stlr    wzr, [x0]       // 带release语义的存储
    ret
ASM_FUNC_END(arm_spin_lock_release)

/* 尝试获取自旋锁 */
ASM_FUNC_BEGIN(arm_spin_lock_try)
    mov     x2, #1
    ldaxr   w1, [x0]        // 带acquire语义的加载独占
    cbnz    w1, 2f          // 如果锁已被持有，返回失败
    stxr    w3, w2, [x0]    // 尝试存储独占
    cbnz    w3, 2f          // 如果存储失败，返回失败
    dmb     ish             // 获取后的内存屏障
    mov     x0, #1          // 返回成功
    ret
2:  mov     x0, #0          // 返回失败
    ret
ASM_FUNC_END(arm_spin_lock_try)

/* 互斥锁获取 */
ASM_FUNC_BEGIN(arm_mutex_lock_acquire)
    mov     x1, #1
1:  ldaxr   w2, [x0, #16]    // 加载owner
    cbnz    w2, 2f           // 已被持有则等待
    stxr    w3, w1, [x0, #16] // 尝试获取锁
    cbnz    w3, 1b           // 失败重试
    ret
2:  // 加入等待队列逻辑
    b       1b
ASM_FUNC_END(arm_mutex_lock_acquire)

/* 互斥锁释放 */
ASM_FUNC_BEGIN(arm_mutex_lock_release)
    dmb     ish              // 内存屏障
    str     wzr, [x0, #16]   // 释放锁
    // 唤醒等待者逻辑
    ret
ASM_FUNC_END(arm_mutex_lock_release)

/* 尝试获取互斥锁 */
ASM_FUNC_BEGIN(arm_mutex_lock_try)
    mov     x1, #1
    ldaxr   w2, [x0, #16]    // 加载owner
    cbnz    w2, 1f           // 已被持有则返回失败
    stxr    w3, w1, [x0, #16] // 尝试获取锁
    cbnz    w3, 1f           // 失败则返回失败
    mov     x0, #1           // 返回成功
    ret
1:  mov     x0, #0           // 返回失败
    ret
ASM_FUNC_END(arm_mutex_lock_try)

/* 读写信号量操作 */
ASM_FUNC_BEGIN(arm_down_read)
    mov     x1, #0x100000000    // RWSEM_READER_BIAS
1:  ldaxr   x2, [x0]           // 原子加载count
    adds    x3, x2, x1          // 增加READER_BIAS
    stxr    w4, x3, [x0]        // 尝试存储
    cbnz    w4, 1b              // 失败重试
    tbnz    x2, #63, 2f        // 检查WRITER_LOCKED位
    ret
2:  dmb     ish                 // 内存屏障
    sub     x2, x2, x1          // 恢复原值
    stlr    x2, [x0]            // 存储count
    b       arm_down_read        // 重试
ASM_FUNC_END(arm_down_read)

ASM_FUNC_BEGIN(arm_up_read)
    mov     x1, #0x100000000    // RWSEM_READER_BIAS
    dmb     ish                 // 内存屏障
1:  ldxr    x2, [x0]           // 加载count
    subs    x3, x2, x1          // 减去READER_BIAS
    stxr    w4, x3, [x0]        // 尝试存储
    cbnz    w4, 1b              // 失败重试
    ret
ASM_FUNC_END(arm_up_read)

ASM_FUNC_BEGIN(arm_down_write)
    mov     x1, #0x8000000000000000 // RWSEM_WRITER_LOCKED
1:  ldaxr   x2, [x0]           // 原子加载count
    tbnz    x2, #63, 1b        // 检查WRITER_LOCKED位
    orr     x3, x2, x1         // 设置WRITER_LOCKED位
    stxr    w4, x3, [x0]       // 尝试存储
    cbnz    w4, 1b             // 失败重试
    ret
ASM_FUNC_END(arm_down_write)

ASM_FUNC_BEGIN(arm_up_write)
    dmb     ish                 // 内存屏障
    mov     x1, #0x8000000000000000 // RWSEM_WRITER_LOCKED
1:  ldxr    x2, [x0]           // 加载count
    bic     x3, x2, x1         // 清除WRITER_LOCKED位
    stxr    w4, x3, [x0]        // 尝试存储
    cbnz    w4, 1b              // 失败重试
    ret
ASM_FUNC_END(arm_up_write)

/* 信号量操作 */
ASM_FUNC_BEGIN(arm_down)
    // 专门的信号量实现
    mov     x1, #1
1:  ldaxr   w2, [x0, #8]     // 加载count
    cbz     w2, 2f           // count为0则等待
    sub     w3, w2, x1       // count减1
    stxr    w4, w3, [x0, #8] // 尝试存储
    cbnz    w4, 1b           // 失败重试
    ret
2:  // 加入等待队列逻辑
    b       1b
ASM_FUNC_END(arm_down)

ASM_FUNC_BEGIN(arm_up)
    // 专门的信号量实现
    mov     x1, #1
1:  ldxr    w2, [x0, #8]     // 加载count
    add     w3, w2, x1       // count加1
    stxr    w4, w3, [x0, #8] // 尝试存储
    cbnz    w4, 1b           // 失败重试
    // 唤醒等待者逻辑
    ret
ASM_FUNC_END(arm_up)

/* RCU操作 */
ASM_FUNC_BEGIN(arm_rcu_read_lock)
    ldr     w1, [x0]            // 加载completed
    add     w1, w1, #1          // 增加1
    str     w1, [x0]            // 存储completed
    dmb     ish                 // 内存屏障
    ret
ASM_FUNC_END(arm_rcu_read_lock)

ASM_FUNC_BEGIN(arm_rcu_read_unlock)
    dmb     ish                 // 内存屏障
    ldr     w1, [x0]            // 加载completed
    sub     w1, w1, #1          // 减少1
    str     w1, [x0]            // 存储completed
    ret
ASM_FUNC_END(arm_rcu_read_unlock)

ASM_FUNC_BEGIN(arm_synchronize_rcu)
    ldr     w1, [x0]            // 加载completed
    dmb     ish                 // 内存屏障
1:  ldr     w2, [x0, #4]       // 加载signalled
    cmp     w2, w1              // 比较
    b.ne    1b                  // 不相等则等待
    ret
ASM_FUNC_END(arm_synchronize_rcu)

/* 顺序锁操作 */
ASM_FUNC_BEGIN(arm_read_seqbegin)
1:  ldaxr   w0, [x0]           // 加载sequence
    tbnz    w0, #0, 1b         // 检查最低位
    dmb     ish                 // 内存屏障
    ret
ASM_FUNC_END(arm_read_seqbegin)

ASM_FUNC_BEGIN(arm_read_seqretry)
    dmb     ish                 // 内存屏障
    ldr     w2, [x0]            // 加载当前sequence
    cmp     w2, w1              // 比较
    cset    w0, ne              // 设置返回值
    ret
ASM_FUNC_END(arm_read_seqretry)

/* 完成量操作 */
ASM_FUNC_BEGIN(arm_wait_for_completion)
    // 基于自旋锁实现
    b       arm_spin_lock_acquire
ASM_FUNC_END(arm_wait_for_completion)

ASM_FUNC_BEGIN(arm_complete)
    // 基于自旋锁实现
    b       arm_spin_lock_release
ASM_FUNC_END(arm_complete)