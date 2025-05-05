#include <common/lock.h>
#include <common/cpuinfo.h>
#include <common/debug.h>
#include <arch/arm/sync.h>
#include "lock.h"
#include "ticket.h"

/* 声明汇编函数 */
extern void arm_spin_lock_acquire(volatile u64 *lock);
extern void arm_spin_lock_release(volatile u64 *lock);
extern int arm_spin_lock_try(volatile u64 *lock);

/* 自旋锁操作 */
void spin_lock_init(struct spin_lock *l) {
    struct lock_impl *lock = (struct lock_impl *)l;
    BUG_ON(!lock);
    /* Initialize ticket lock */
    lock->owner = 0;
    lock->next = 0;
    smp_wmb();
}

void spin_lock_acquire(struct spin_lock *lock) {
    arm_spin_lock_acquire(&lock->spin_slock);
}

void spin_lock_release(struct spin_lock *lock) {
    arm_spin_lock_release(&lock->spin_slock);
}

int spin_lock_try(struct spin_lock *lock) {
    return arm_spin_lock_try(&lock->spin_slock);
}

/* 声明互斥锁汇编函数 */
extern void arm_mutex_lock_acquire(volatile u64 *lock);
extern void arm_mutex_lock_release(volatile u64 *lock);
extern int arm_mutex_lock_try(volatile u64 *lock);

/* 互斥锁操作 */
void mutex_lock_init(struct mutex_lock *lock) {
    spin_lock_init(&lock->lock);
    INIT_LIST_HEAD(&lock->wait_list);
    lock->owner = 0;
}

void mutex_lock_acquire(struct mutex_lock *lock) {
    arm_mutex_lock_acquire(&lock->lock.spin_slock);
}

void mutex_lock_release(struct mutex_lock *lock) {
    arm_mutex_lock_release(&lock->lock.spin_slock);
}

int mutex_lock_try(struct mutex_lock *lock) {
    return arm_mutex_lock_try(&lock->lock.spin_slock);
}

/* 声明读写信号量汇编函数 */
extern void arm_down_read(rw_semaphore *sem);
extern void arm_up_read(rw_semaphore *sem);
extern void arm_down_write(rw_semaphore *sem);
extern void arm_up_write(rw_semaphore *sem);

/* 读写信号量操作 */
void rw_semaphore_init(rw_semaphore *sem) {
    atomic_set(&sem->count, 0);
    spin_lock_init(&sem->wait_lock);
    INIT_LIST_HEAD(&sem->wait_list);
}

void down_read(rw_semaphore *sem) {
    arm_down_read(sem);
}

void up_read(rw_semaphore *sem) {
    arm_up_read(sem);
}

void down_write(rw_semaphore *sem) {
    arm_down_write(sem);
}

void up_write(rw_semaphore *sem) {
    arm_up_write(sem);
}

/* 声明信号量汇编函数 */
extern void arm_down(semaphore *sem);
extern void arm_up(semaphore *sem);

/* 信号量操作 */
void semaphore_init(semaphore *sem, u64 count) {
    spin_lock_init(&sem->lock);
    sem->count = count;
    INIT_LIST_HEAD(&sem->wait_list);
}

void down(semaphore *sem) {
    arm_down(sem);
}

void up(semaphore *sem) {
    arm_up(sem);
}

/* 声明RCU汇编函数 */
extern void arm_rcu_read_lock(rcu_lock *lock);
extern void arm_rcu_read_unlock(rcu_lock *lock);
extern void arm_synchronize_rcu(rcu_state *state);

/* RCU操作 */
void rcu_read_lock(rcu_lock *lock) {
    arm_rcu_read_lock(lock);
}

void rcu_read_unlock(rcu_lock *lock) {
    arm_rcu_read_unlock(lock);
}

void synchronize_rcu(rcu_state *state) {
    arm_synchronize_rcu(state);
}

/* 声明顺序锁汇编函数 */
extern u32 arm_read_seqbegin(seq_lock *lock);
extern int arm_read_seqretry(seq_lock *lock, u32 seq);

/* 顺序锁操作 */
void seq_lock_init(seq_lock *lock) {
    lock->sequence = 0;
    spin_lock_init(&lock->lock);
}

u32 read_seqbegin(seq_lock *lock) {
    return arm_read_seqbegin(lock);
}

int read_seqretry(seq_lock *lock, u32 seq) {
    return arm_read_seqretry(lock, seq);
}

/* 声明完成量汇编函数 */
extern void arm_wait_for_completion(completion *comp);
extern void arm_complete(completion *comp);

/* 完成量操作 */
void completion_init(completion *comp) {
    comp->done = 0;
    spin_lock_init(&comp->wait_lock);
    INIT_LIST_HEAD(&comp->wait_list);
}

void wait_for_completion(completion *comp) {
    arm_wait_for_completion(comp);
}

void complete(completion *comp) {
    arm_complete(comp);
}

struct spin_lock big_kernel_lock;
void kernel_lock_init(void)
{
    u32 ret = 0;
    ret = lock_init(&big_kernel_lock);
    BUG_ON(ret != 0);
}
void lock_kernel(void)
{
    arm_spin_lock_acquire(&big_kernel_lock);
}
void unlock_kernel(void)
{
    BUG_ON(!is_locked(&big_kernel_lock));
    arm_spin_lock_release(&big_kernel_lock);
}