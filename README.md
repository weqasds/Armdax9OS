# 操作系统毕设
## 项目概述
## 项目结构.
```bash
|-- CMakeLists.txt
|-- CMakePresets.json
|-- arch #架构
|   |-- CMakeLists.txt
|   |-- arm
|   |-- generic
|   `-- start.S
|-- include #内核与架构头文件定义
|   |-- arch
|   |   `-- arm
|   |-- common
|   |-- debug
|   |-- device
|   |-- fs
|   |-- ipc
|   |-- irq
|   |-- mm
|   |-- object
|   |-- sched
|   |-- syscall
|   `-- time
|-- keneral #内核源代码
|   |-- CMakeLists.txt #内核构建
|   |-- ReadMe.md #内核总览信息
|   |-- debug #用于debug的调试函数
|   |-- device #设备抽象
|   |-- elf #可执行文件描述与相关加载函数
|   |-- fs #文件系统
|   |-- ipc #进程间调度
|   |-- irq #异常中断处理的抽象
|   |-- main.c #内核入口文件
|   |-- mm #内存管理
|   |-- object #内核对象管理以及能力组
|   |-- process #进程与线程管理 
|   |-- sched #处理机调度
|   |-- syscall #存放系统调用
|   `-- time #分时
|-- libs #常用的静态库，
|   `-- CMakeLists.txt
`-- user #用户空间实用程序
    `-- CMakeLists.txt
```
