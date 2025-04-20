//kernel printf
#pragma once

#include <lib/printf.h>

#define WARNING 0
#define INFO    1
#define DEBUG   2

/* LOG_LEVEL is INFO by default */

#if LOG_LEVEL >= WARNING
#define kwarn(fmt, ...) printf("[WARN] file:%s " fmt, __FILE__, ##__VA_ARGS__)
#else
#define kwarn(fmt, ...)
#endif

#if LOG_LEVEL >= INFO
#define kinfo(fmt, ...) printf("[INFO] " fmt, ##__VA_ARGS__)
#else
#define kinfo(fmt, ...)
#endif

#if LOG_LEVEL >= DEBUG
#define kdebug(fmt, ...) printf("[DEBUG] " fmt, ##__VA_ARGS__)
#else
#define kdebug(fmt, ...)
#endif