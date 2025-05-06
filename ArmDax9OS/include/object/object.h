#pragma once
#include <arch/arm/plat/raspi3/machine.h>
#include <common/types.h>
#include <mm/vmspace.h>
#include <sched/sched.h>
#include <sched/context.h>
#include <arch/arm/machine/smp.h>
#include <common/lock.h>
#include <common/types.h>

/* 进程/线程ID类型 */
typedef s64 tid_t;
typedef s64 capid_t;

/* 进程状态结构 */
struct proc_status {
    pid_t pid;
    tid_t main_tid;
    u32 thread_count;
    u32 state;
    u64 memory_usage;
};

/* 线程状态结构 */
struct thread_status {
    tid_t tid;
    pid_t pid;
    u32 state;
    u32 cpu_affinity;
};
extern struct process *current_process[PLAT_CPU_NUM];
extern struct process idle_process[PLAT_CPU_NUM];
#define current_process (current_process[get_gpu_id()])
#define DEFAULT_KERNEL_STACK_SZ (0x1000)


struct process
{
    /// @brief 进程上下文结构
    struct process_context *pct;
    /// @brief 线程链表
    struct list_head thread_list;
    /// @brief 线程数量
    u32 thread_cnt;
    struct spin_lock process_lock;
    /// @brief 进程地址空间
    struct vmspace space;
    /// @brief 进程id
    pid_t pid;
    /// @brief 进程主线程id
    s64 tpid;
    

    
};

struct thread
{
    /// @brief 同一进程的所有线程
    struct list_head node;
    /// @brief 就绪队列中的线程
    struct list_head ready_queue_node;
    /// @brief 信号量等待队列线程
    struct list_head sem_queue_node;
    /// @brief 线程上下文结构
    struct thread_context *thread_ctx;
    /// @brief 所属父进程
    struct process *parent_process;
    /// @brief 上一个运行线程
    struct thread *prev_thread;
};
/* 根进程初始化 */
void create_root_process(void);
void switch_process_vmspace_to(struct process *);

/* 进程管理接口 */
int proc_create(pid_t *pid, vaddr_t entry, vaddr_t stack, size_t stack_size);
void proc_exit(int exit_code);
int proc_wait(pid_t pid, int *exit_status);
int proc_kill(pid_t pid);
int proc_status(pid_t pid, struct proc_status *status);

/* 线程管理接口 */
int thread_create(tid_t *tid, vaddr_t entry, vaddr_t stack, size_t stack_size);
void thread_exit(int exit_code);
int thread_join(tid_t tid, int *exit_status);
int thread_status(tid_t tid, struct thread_status *status);


/* 特殊线程操作 */
int clone_thread(tid_t *tid, struct thread_context *ctx, vaddr_t stack);



