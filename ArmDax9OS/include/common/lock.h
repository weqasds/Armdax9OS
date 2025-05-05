#pragma once
#include <common/types.h>
#include <common/list.h>
#include <common/debug.h>
#include <common/cpuinfo.h>
/**
 * 定义一系列锁结构，用于其他文件使用
 */
//自旋锁
typedef struct spin_lock
{
    volatile u64 spin_slock;
#ifdef CONFIG_DEBUG_SPINLOCK
    u32 magic,owner_cpu;
    void* owner;
#endif
#ifdef CONFIG_LOCK_DEBUG
    lockdep_map_t dep_map;
    const char* name;
#endif
}spin_lock;

//互斥锁
typedef struct mutex_lock{
    struct spin_lock lock;
    struct list_head wait_list;
    atomic_t owner;
#ifdef CONFIG_DEBUG-MUTEXLOCK
    const u8 *name;
    void* magic;
#endif
#ifdef CONFIG_LOCK_DEBUG
    lockdep_map_t dep_map;
    const char* name;
#endif
} mutex_lock;

typedef struct rw_semaphore
{
    atomic_long_t count;
    struct spin_lock wait_lock;
    list_head wait_list;
#ifdef CONFIG_LOCK_DEBUG
    lockdep_map_t dep_map;
    const char* name;
#endif
}rw_semaphore;

typedef struct semaphore
{
    struct spin_lock lock;
    u64 count;
    list_head wait_list;
#ifdef CONFIG_LOCK_DEBUG
    lockdep_map_t dep_map;
    const char* name;
#endif
}semaphore;
typedef struct rcu_head
{
    struct rcu_head *next;
    //函数指针
    void(*func)(struct rcu_head*);
}rcu_head;

typedef struct rcu_lock
{
    u32 completed;
    rcu_head *pending;
    struct spin_lock lock;
    u32 length; 
}rcu_lock;
//rcu全局状态
typedef struct rcu_state
{
    unsigned long   completed;     // 全局完成的批次号
    atomic_t        signalled;     // 宽限期信号标志
    struct spin_lock lock;          // 全局锁
    rcu_lock        __percpu *rda; // 指向每CPU数据的指针
#ifdef CONFIG_RCU_DEBUG
    const char      *name;         // 调试用名称
#endif
}rcu_state;

typedef struct seq_lock
{
    u32 sequence;
    struct spin_lock lock;
#ifdef CONFIG_LOCK_DEBUG
    lockdep_map_t dep_map;
    const char* name;
#endif
}seq_lock;
typedef struct completion
{
    u32 done;
    struct spin_lock wait_lock;
    list_head wait_list;
}completion;

//用户空间锁(待完成)
typedef struct futex
{
    /* data */
}futex;

/* 锁操作函数声明 */
void spin_lock_init(struct spin_lock *lock);
void spin_lock_acquire(struct spin_lock *lock);
void spin_lock_release(struct spin_lock *lock);
int spin_lock_try(struct spin_lock *lock);  // 使用int替代bool

void mutex_lock_init(struct mutex_lock *lock);
void mutex_lock_acquire(struct mutex_lock *lock);
void mutex_lock_release(struct mutex_lock *lock);
int mutex_lock_try(struct mutex_lock *lock);  // 使用int替代bool

void rw_semaphore_init(rw_semaphore *sem);
void down_read(rw_semaphore *sem);
void up_read(rw_semaphore *sem);
void down_write(rw_semaphore *sem);
void up_write(rw_semaphore *sem);

void semaphore_init(semaphore *sem, u64 count);
void down(semaphore *sem);
void up(semaphore *sem);

void rcu_read_lock(rcu_lock *lock);
void rcu_read_unlock(rcu_lock *lock);
void synchronize_rcu(rcu_state *state);

void seq_lock_init(seq_lock *lock);
u32 read_seqbegin(seq_lock *lock);
int read_seqretry(seq_lock *lock, u32 seq);

void completion_init(completion *comp);
void wait_for_completion(completion *comp);
void complete(completion *comp);


//全局锁代码
extern struct spin_lock big_kernel_lock;
void kernel_lock_init(void);
void lock_kernel(void);
void unlock_kernel(void);
