#pragma once 
//用于调试锁
#ifdef CONFIG_LOCK_DEBUG
#define _LOCK_DEBUG_INITIALIZER(lockname) \
    .dep_map = { .name = #lockname }, \
    .name = #lockname
#else
#define _LOCK_DEBUG_INITIALIZER(lockname)
#endif

typedef struct lockdep_map {
    const char *name;
    void *key;
} lockdep_map_t;
