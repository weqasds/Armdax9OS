# 操作系统毕设
## 项目结构
```bash
ArmDax9OS/
├── Makefile              # 主Makefile，管理整个项目的构建
├── link/                 # 编译链接脚本
├── scripts/              # 构建脚本和工具脚本
│   ├── toolchain.mk      # 交叉编译工具链配置
│   └── qemu-run.sh       # QEMU模拟器运行脚本
│
├── arch/                 # 体系结构相关代码
│   ├── arm/              # ARM架构特定实现
│       ├── include/      # 架构相关头文件
│       ├── exceptions/   # 异常处理
│       ├── mmu/          # MMU初始化
│       └── smp.c         # 多核启动
│   └── boot.S            # 汇编启动代码
│
├── kernel/               # 内核核心代码
│   ├── main.c            # 内核主入口
│   ├── syscall/          # 系统调用集成
│   ├── include/          # 内核头文件
│   │   ├── mm.h          # 内存管理
│   │   ├── sched.h       # 进程调度
│   │   └── ...
│   │
│   ├── mm/               # 内存管理子系统
│   │   ├── page_alloc.c  # 页分配器
│   │   └── slab.c        # Slab分配器
│   │
│   ├── sched/            # 进程调度
│   │   ├── scheduler.c   # 调度算法实现
│   │   └── context.S     # 上下文切换汇编
│   │
│   ├── fs/               # 文件系统
│   │   ├── vfs.c         # 虚拟文件系统
│   │   └── devfs/        # 设备文件系统
│   │
│   └── drivers/          # 设备驱动
│       ├── uart.c        # 串口驱动
│       ├── timer.c       # 定时器驱动
│       └── ...
│
│
├── lib/                  # 基础库
│   ├── string.c         # 字符串操作
│   ├── printf.c         # 格式化输出
│   └── ...             
│
├── user/                # 用户空间程序（可选）
│   ├── init.c           # 初始化进程
│   └── ...
│
├── build/               # 构建输出目录
├── doc/                 # 设计文档
│   └── design.md       # 系统设计文档
│
└── third_party/         # 第三方库
    └── ...             
```