#pragma once 
#include <common/types.h>

//定义内核常用变量
/* Leave 8K space to kernel stack */
//内核栈大小
#define KERNEL_STACK_SIZE (8192)
//从核栈指针大小
#define IDLE_STACK_SIZE   (8192)
//栈按16字节对齐
#define STACK_ALIGNMENT   16

// #include <arch/mmu.h>
/* can be different in different architectures */
#ifndef KBASE
//64位架构的内核起点
#define KBASE 0xffffff0000000000
#endif