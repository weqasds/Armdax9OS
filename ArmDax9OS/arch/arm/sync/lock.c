#include <common/lock.h>
#include <common/cpuinfo.h>
#include <common/debug.h>
#include <arch/arm/sync.h>
#include "lock.h"

/* 自旋锁操作 */
void spin_lock_init(struct spin_lock *lock) {
    lock->spin_slock = 0;
}

void spin_lock_acquire(struct spin_lock *lock) {
    arm_spin_lock((volatile u64 *)&lock->spin_slock);
}

void spin_lock_release(struct spin_lock *lock) {
    arm_spin_unlock((volatile u64 *)&lock->spin_slock);
}

int spin_lock_try(struct spin_lock *lock) {
    return arm_spin_trylock((volatile u64 *)&lock->spin_slock);
}

/* 互斥锁操作 */
void mutex_lock_init(struct mutex_lock *lock) {
    spin_lock_init(&lock->lock);
    INIT_LIST_HEAD(&lock->wait_list);
    lock->owner = 0;
}

void mutex_lock_acquire(struct mutex_lock *lock) {
    spin_lock_acquire(&lock->lock);
    while (lock->owner) {
        // 加入等待队列
        spin_lock_release(&lock->lock);
        spin_lock_acquire(&lock->lock);
    }
    lock->owner = 1;
    spin_lock_release(&lock->lock);
}

void mutex_lock_release(struct mutex_lock *lock) {
    spin_lock_acquire(&lock->lock);
    lock->owner = 0;
    spin_lock_release(&lock->lock);
}

int mutex_lock_try(struct mutex_lock *lock) {
    spin_lock_acquire(&lock->lock);
    if (lock->owner) {
        spin_lock_release(&lock->lock);
        return 0;
    }
    lock->owner = 1;
    spin_lock_release(&lock->lock);
    return 1;
}

/* 读写信号量操作 */
void rw_semaphore_init(rw_semaphore *sem) {
    atomic_set(&sem->count, 0);
    spin_lock_init(&sem->wait_lock);
    INIT_LIST_HEAD(&sem->wait_list);
}

#define RWSEM_READER_BIAS    (1UL << 32)
#define RWSEM_WRITER_LOCKED  (1UL << 63)

void down_read(rw_semaphore *sem) {
    u64 count = atomic_long_add_return(RWSEM_READER_BIAS, &sem->count);
    if (count & RWSEM_WRITER_LOCKED) {
        spin_lock_acquire(&sem->wait_lock);
        atomic_long_sub(RWSEM_READER_BIAS, &sem->count);
        // 加入等待队列
        spin_lock_release(&sem->wait_lock);
        down_read(sem); // 重试
    }
}

void up_read(rw_semaphore *sem) {
    u64 count = atomic_long_sub_return(RWSEM_READER_BIAS, &sem->count);
    if (count < RWSEM_READER_BIAS && (count & RWSEM_READER_BIAS) == 0) {
        // 唤醒等待的写者
        spin_lock_acquire(&sem->wait_lock);
        if (!list_empty(&sem->wait_list)) {
            // 唤醒第一个等待者
        }
        spin_lock_release(&sem->wait_lock);
    }
}

void down_write(rw_semaphore *sem) {
    spin_lock_acquire(&sem->wait_lock);
    while (1) {
        u64 count = atomic_long_read(&sem->count);
        if ((count & RWSEM_WRITER_LOCKED) == 0) {
            if (atomic_long_cmpxchg(&sem->count, count, 
                                  count | RWSEM_WRITER_LOCKED) == count) {
                break;
            }
        }
        // 加入等待队列
    }
    spin_lock_release(&sem->wait_lock);
}

void up_write(rw_semaphore *sem) {
    spin_lock_acquire(&sem->wait_lock);
    atomic_long_andnot(RWSEM_WRITER_LOCKED, &sem->count);
    if (!list_empty(&sem->wait_list)) {
        // 唤醒等待者
    }
    spin_lock_release(&sem->wait_lock);
}

/* 信号量操作 */
void semaphore_init(semaphore *sem, u64 count) {
    spin_lock_init(&sem->lock);
    sem->count = count;
    INIT_LIST_HEAD(&sem->wait_list);
}

void down(semaphore *sem) {
    spin_lock_acquire(&sem->lock);
    while (sem->count == 0) {
        // 加入等待队列
        spin_lock_release(&sem->lock);
        spin_lock_acquire(&sem->lock);
    }
    sem->count--;
    spin_lock_release(&sem->lock);
}

void up(semaphore *sem) {
    spin_lock_acquire(&sem->lock);
    sem->count++;
    if (!list_empty(&sem->wait_list)) {
        // 唤醒等待者
    }
    spin_lock_release(&sem->lock);
}

/* RCU操作 */
void rcu_read_lock(rcu_lock *lock) {
    atomic_inc(&lock->completed);
    smp_mb();
}

void rcu_read_unlock(rcu_lock *lock) {
    smp_mb();
    atomic_dec(&lock->completed);
}

void synchronize_rcu(rcu_state *state) {
    u32 cur = state->completed;
    smp_mb();
    while (atomic_read(&state->signalled) != cur) {
        cpu_relax();
    }
}

/* 顺序锁操作 */
void seq_lock_init(seq_lock *lock) {
    lock->sequence = 0;
    spin_lock_init(&lock->lock);
}

u32 read_seqbegin(seq_lock *lock) {
    u32 seq;
    do {
        seq = lock->sequence;
        smp_rmb();
    } while (seq & 1);
    return seq;
}

int read_seqretry(seq_lock *lock, u32 seq) {
    smp_rmb();
    return lock->sequence != seq;
}

/* 完成量操作 */
void completion_init(completion *comp) {
    comp->done = 0;
    spin_lock_init(&comp->wait_lock);
    INIT_LIST_HEAD(&comp->wait_list);
}

void wait_for_completion(completion *comp) {
    spin_lock_acquire(&comp->wait_lock);
    while (!comp->done) {
        // 加入等待队列
        spin_lock_release(&comp->wait_lock);
        spin_lock_acquire(&comp->wait_lock);
    }
    spin_lock_release(&comp->wait_lock);
}

void complete(completion *comp) {
    spin_lock_acquire(&comp->wait_lock);
    comp->done = 1;
    if (!list_empty(&comp->wait_list)) {
        // 唤醒等待者
    }
    spin_lock_release(&comp->wait_lock);
}
