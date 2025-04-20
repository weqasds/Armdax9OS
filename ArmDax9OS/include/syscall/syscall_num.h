#pragma once

//the syscall max nums
#define NR_SYSCALL 255

/* Arm v8a 架构基础系统调用号定义 */
#define SYS_exit              0   // 进程/线程正常退出，释放资源
#define SYS_yield             1   // 主动让出 CPU，进入调度器
#define SYS_get_time          2   // 获取当前系统时间（纳秒或毫秒）
#define SYS_nanosleep         3   // 线程休眠一段时间（纳秒）
#define SYS_get_tid           4   // 获取当前线程 ID
#define SYS_get_pid           5   // 获取当前进程 ID
#define SYS_get_cpu_id        6   // 获取当前运行的 CPU 核编号
#define SYS_log_write         7   // 向内核日志设备写入调试信息
#define SYS_debug_break       8   // 触发调试断点或调试陷入（调试用途）
#define SYS_syscall_info      9   // 查询系统调用元信息（用于调试）

//process manage
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

#define SYS_capgroup_create   30  // 创建新的能力组（返回 capgroup_id）
#define SYS_capgroup_destroy  31  // 销毁指定能力组（需无成员或强制）
#define SYS_capgroup_add_proc 32  // 将某个进程加入指定能力组
#define SYS_capgroup_del_proc 33  // 将某个进程移出能力组
#define SYS_capgroup_set_attr 34  // 设置能力组属性（如权限位）
#define SYS_capgroup_get_attr 35  // 获取能力组属性
#define SYS_capgroup_list     36  // 枚举当前系统的能力组信息

//memory manage
#define SYS_mmap              40  // 映射虚拟内存区域
#define SYS_munmap            41  // 解除某个虚拟地址区间的映射
#define SYS_mprotect          42  // 修改页权限
#define SYS_pmo_create        34  // 创建物理内存对象（PMO）
#define SYS_pmo_destory       35  // 销毁 PMO
#define SYS_pmo_info          36  // 获取 PMO 信息
#define SYS_pmo_write         37  // 向 PMO 写数据
#define SYS_pmo_read          38  // 从 PMO 读数据
#define SYS_page_create       43  // 创建一个物理内存对象（PMObject），返回其 ID
#define SYS_page_destroy      44  // 销毁 PMO，释放物理内存资源
#define SYS_page_write        45  // 向指定 page 写入数据
#define SYS_page_read         46  // 从指定 page 读取数据
#define SYS_map_page          47  // 将指定 page 映射到给定虚拟地址（带偏移、权限）
#define SYS_vmspace_create    49  // 创建一个新的虚拟地址空间（用于新进程）
#define SYS_vmspace_destroy   50  // 销毁虚拟地址空间
#define SYS_vmarea_query      51  // 查询某虚拟地址对应的 VMA 信息
#define SYS_vmarea_protect    52  // 修改 VMA 区间的访问权限
#define SYS_vmspace_switch    53  // 切换当前进程的地址空间（用于 exec）
#define SYS_get_vmspace       54  // 获取当前进程绑定的 VMASpace ID

//sched
#define SYS_sched_yield       70  // 主动让出调度权
#define SYS_sched_set_policy  71  // 设置线程调度策略（如 RR, FIFO）
#define SYS_sched_get_policy  72  // 获取线程调度策略
#define SYS_sched_set_prio    73  // 设置线程优先级
#define SYS_sched_get_prio    74  // 获取线程优先级
#define SYS_sched_bind_cpu    75  // 将线程绑定到指定 CPU
#define SYS_sched_migrate     76  // 将线程迁移到另一个 CPU

#define SYS_signal_register   80  // 注册用户态信号处理函数地址
#define SYS_signal_raise      81  // 向线程发送信号
#define SYS_signal_return     82  // 从信号处理函数返回（恢复上下文）
#define SYS_signal_mask       83  // 屏蔽指定信号
#define SYS_signal_unmask     84  // 解除信号屏蔽
#define SYS_set_exception_handler 85  // 注册用户态异常处理函数（如页错陷入）

//file 
#define SYS_open              90  // 打开文件，返回文件描述符
#define SYS_close             91  // 关闭文件描述符
#define SYS_read              92  // 从文件中读取数据
#define SYS_write             93  // 向文件写入数据
#define SYS_lseek             94  // 设置文件读写指针
#define SYS_fstat             95  // 获取文件状态信息
#define SYS_mmap_file         96  // 将文件映射到进程地址空间（私有或共享）
#define SYS_munmap_file       97  // 解除文件映射
#define SYS_dup               98  // 复制文件描述符
#define SYS_pipe              99  // 创建管道（双文件描述符）
#define SYS_ioctl             100 // 对设备或文件执行控制命令

/*Exception*/ 
#define SYS_irq_register        101
#define SYS_irq_wait            102
#define SYS_irq_ack             103
#define SYS_user_fault_register 104
#define SYS_user_fault_map      105