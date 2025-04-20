#pragma once
#ifndef __ASM__
#include <common/asm.h>
#endif
//字节对齐
#define ALIGN(n) __attribute__((__aligned__(n)))

//内存对齐操作
#define ROUND_UP(x, n)     (((x) + (n)-1) & ~((n)-1))
#define ROUND_DOWN(x, n)   ((x) & ~((n)-1))
#define DIV_ROUND_UP(n, d) (((n) + (d)-1) / (d))


//判断是否对齐
#define IS_ALIGNED(x, a) (((x) & ((typeof(x))(a)-1)) == 0)

//位操作：生成第x位为1的掩码 
#define BIT(x) (1ULL << (x))

/* 结构体成员偏移量计算（经典Linux内核实现） */
#define offsetof(TYPE, MEMBER) ((u64) & ((TYPE *)0)->MEMBER)
/* 通过成员指针获取容器结构体指针（类型安全的逆向计算） */
#define container_of(ptr, type, field) \
        (                               \
            (type *)                    \
            (                           \
                (void *)(ptr) - (u64)(&(((type *)(0))->field)) \
            ) \
        )
/* 带空指针检查的container_of版本 */
#define container_of_safe(ptr, type, field)                     \
        ({                                                      \
                typeof(ptr) __ptr = (ptr);                      \
                type *__obj = container_of(__ptr, type, field); \
                (__ptr ? __obj : NULL);                         \
        })

#define MAX(x, y) ((x) < (y) ? (y) : (x))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
//触发致命错误
#define BUG_ON(expr)                                        \
        do {                                                \
                if ((expr)) {                               \
                        printk("BUG: %s:%d on (expr) %s\n", \
                               __func__,                    \
                               __LINE__,                    \
                               #expr);                      \
                        for (;;) {                          \
                        }                                   \
                }                                           \
        } while (0)
/* 自定义错误信息的内核致命错误 */
#define BUG(str, ...)                          \
        do {                                   \
                printk("BUG: %s:%d " str "\n", \
                       __func__,               \
                       __LINE__,               \
                       ##__VA_ARGS__);         \
                for (;;) {                     \
                }                              \
        } while (0)
/* 非致命警告机制（继续执行） */
#define WARN(msg) printk("WARN: %s:%d %s\n", __func__, __LINE__, msg)
/* 条件触发的警告机制 */
#define WARN_ON(cond, msg)                                      \
        do {                                                    \
                if ((cond)) {                                   \
                        printk("WARN: %s:%d %s on " #cond "\n", \
                               __func__,                        \
                               __LINE__,                        \
                               msg);                            \
                }                                               \
        } while (0)





#ifdef __GNUC__
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x)   (!!(x))
#define unlikely(x) (!!(x))
#endif