#pragma once

//the syscall max nums
#define NR_SYSCALL 256  // 调整为128个系统调用空间

/* Arm v8a 架构基础系统调用号定义 */
#define SYS_exit              0   // 进程/线程正常退出，释放资源
#define SYS_yield             1   // 主动让出 CPU，进入调度器
#define SYS_get_time          2   // 获取当前系统时间（纳秒或毫秒）
#define SYS_nanosleep         3   // 线程休眠一段时间（纳秒）
#define SYS_get_tid           4   // 获取当前线程 ID
#define SYS_get_pid           5   // 获取当前进程 ID
#define SYS_get_cpu_id        6   // 获取当前运行的 CPU 核编号
// 预留7-9

//process manage (10-29)
#define SYS_proc_create       10  // 创建一个新进程
#define SYS_proc_exit         11  // 当前进程退出（包含所有线程）
#define SYS_proc_wait         12  // 等待子进程退出，获取退出状态
#define SYS_proc_kill         13  // 向目标进程发送终止请求
#define SYS_thread_create     14  // 创建一个新线程（共享进程资源）
#define SYS_thread_exit       15  // 当前线程退出
#define SYS_thread_join       16  // 等待另一个线程结束
#define SYS_get_capgroup      17  // 获取当前进程所属能力组 ID
#define SYS_set_capgroup      18  // 将进程加入指定能力组（需权限）
#define SYS_clone_thread      19  // 克隆一个线程，指定上下文和栈
#define SYS_proc_status       20  // 查询进程状态信息
#define SYS_thread_status     21  // 查询线程状态信息
// 预留22-29

//memory manage (30-49)
#define SYS_mmap              30  // 映射虚拟内存区域
#define SYS_munmap            31  // 解除某个虚拟地址区间的映射
#define SYS_mprotect          32  // 修改页权限
#define SYS_vmspace_create    33  // 创建一个新的虚拟地址空间（用于新进程）
#define SYS_vmspace_destroy   34  // 销毁虚拟地址空间
#define SYS_vmarea_query      35  // 查询某虚拟地址对应的 VMA 信息
#define SYS_vmarea_protect    36  // 修改 VMA 区间的访问权限
#define SYS_vmspace_switch    37  // 切换当前进程的地址空间（用于 exec）
#define SYS_get_vmspace       38  // 获取当前进程绑定的 VMASpace ID
// 预留39-49

//sched (50-59)
#define SYS_sched_yield       50  // 主动让出调度权
#define SYS_sched_set_policy  51  // 设置线程调度策略（如 RR, FIFO）
#define SYS_sched_get_policy  52  // 获取线程调度策略
#define SYS_sched_set_prio    53  // 设置线程优先级
#define SYS_sched_get_prio    54  // 获取线程优先级
#define SYS_sched_bind_cpu    55  // 将线程绑定到指定 CPU
#define SYS_sched_migrate     56  // 将线程迁移到另一个 CPU
// 预留57-59

//IPC (60-79)
#define SYS_ipc_send          60  // 发送IPC消息
#define SYS_ipc_recv          61  // 接收IPC消息
#define SYS_ipc_call          62  // 同步IPC调用
#define SYS_port_create       63  // 创建通信端口
#define SYS_port_destroy      64  // 销毁通信端口
#define SYS_port_connect      65  // 连接通信端口
// 预留66-79

//signal/exception (80-99)
#define SYS_signal_register   80  // 注册用户态信号处理函数地址
#define SYS_signal_raise      81  // 向线程发送信号
#define SYS_signal_return     82  // 从信号处理函数返回（恢复上下文）
#define SYS_signal_mask       83  // 屏蔽指定信号
#define SYS_signal_unmask     84  // 解除信号屏蔽
#define SYS_set_exception_handler 85  // 注册用户态异常处理函数（如页错陷入）
// 预留86-99

//other (100-127)
#define SYS_irq_register      100 // 注册IRQ处理函数
#define SYS_irq_wait          101 // 等待IRQ发生
#define SYS_irq_ack           102 // 确认IRQ处理完成
#define SYS_user_fault_register 103 // 注册用户态缺页处理函数
#define SYS_user_fault_map    104 // 映射用户态缺页处理区域
// 预留105-127
#define SYS_putc 105
#define SYS_getc 106
/* Performance Benchmark */
#define SYS_perf_start 107
#define SYS_perf_end   108
/* Hardware Access (Privileged Instruction) */
/* - cache */
#define SYS_cache_flush 109
/* - timer */
#define SYS_get_current_tick 110