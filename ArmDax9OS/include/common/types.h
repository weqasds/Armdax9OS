#pragma once
#include <common/macro.h>
// Basic types
typedef unsigned long long u64;
typedef unsigned int u32;

typedef unsigned short u16;
typedef unsigned char u8;
typedef long long s64;
typedef int s32;
typedef short s16;
typedef signed char s8;
typedef unsigned char bool_;
/// @brief 物理地址类型
typedef u64 paddr_t;
/// @brief 虚拟地址类型
typedef u64 vaddr_t;
//原子类型定义
typedef u32 atomic_t;
typedef u64 atomic_long_t; 
typedef u64 atomic_cancel;
#define CACHELINE_SZ         64
#define pad_to_cache_line(n) (ROUND_UP(n, CACHELINE_SZ) - (n))
#define NULL ((void*)0)
#define true (1)
#define false (0)
/* According to `https://pubs.opengroup.org/onlinepubs/9699919799/`. */

/* Used for file block counts. */
typedef s64 blkcnt_t;

/* Used for block sizes. */
typedef s64 blksize_t;

/* Used for system times in clock ticks or CLOCKS_PER_SEC; see <time.h>. */
typedef u64 clock_t;

/* Used for clock ID type in the clock and timer functions.*/
typedef u64 clockid_t;

/* Used for device IDs. */
typedef u64 dev_t;

/* Used for file system block counts. */
typedef u64 fsblkcnt_t;

/* Used for file system file counts. */
typedef u64 fsfilcnt_t;

/* Used for group IDs. */
typedef u64 gid_t;

/*
 * Used as a general identifier; can be used to contain at least a pid_t,
 * uid_t, or gid_t.
 */
typedef u64 id_t;

/* Used for file serial numbers. */
typedef u64 ino_t;

/* Used for XSI interprocess communication. */
typedef u64 key_t;

/* Used for some file attributes. */
typedef u64 mode_t;

/* Used for link counts. */
typedef u64 nlink_t;

/* Used for file sizes. */
typedef s64 off_t;

/* Used for process IDs and process group IDs. */
typedef s64 pid_t;

/* Used to identify a thread attribute object. */
typedef u64 pthread_attr_t;

/* Used to identify a barrier. */
typedef u64 pthread_barrier_t;

/* Used to define a barrier attributes object. */
typedef u64 pthread_barrierattr_t;

/* Used for condition variables. */
typedef u64 pthread_cond_t;

/* Used to identify a condition attribute object. */
typedef u64 pthread_condattr_t;

/* Used for thread-specific data keys. */
typedef u64 pthread_key_t;

/* Used for mutexes. */
typedef u64 pthread_mutex_t;

/* Used to identify a mutex attribute object. */
typedef u64 pthread_mutexattr_t;

/* Used for dynamic package initialization. */
typedef u64 pthread_once_t;

/* Used for read-write locks. */
typedef u64 pthread_rwlock_t;

/* Used for read-write lock attributes. */
typedef u64 pthread_rwlockattr_t;

/* Used to identify a spin lock. */
typedef u64 pthread_spinlock_t;

/* Used to identify a thread. */
typedef u64 pthread_t;

/* Used for sizes of objects. */
typedef u64 size_t;

/* Used for a count of bytes or an error indication. */
typedef s64 ssize_t;

/* Used for time in microseconds. */
typedef s64 suseconds_t;

/* Used for time in seconds. */
typedef u64 time_t;

/* Used for timer ID returned by timer_create(). */
typedef u64 timer_t;

/* Used to identify a trace stream attributes object. */
typedef u64 trace_attr_t;

/* Used to identify a trace event type. */
typedef u64 trace_event_id_t;

/* Used to identify a trace event type set. */
typedef u64 trace_event_set_t;

/* Used to identify a trace stream. */
typedef u64 trace_id_t;

/* Used for user IDs. */
typedef u64 uid_t;
