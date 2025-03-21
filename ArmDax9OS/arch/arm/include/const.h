/*
存放各种arm常量
*/
#ifndef CONST_H
#define CONST_H
//定义初始栈大小
#define INIT_STACK_SIZE 0x2000
#define KENERAL_VADDR 0xffffff0000000000
#define CONFIG_ARM64_TEXT_OFFSET 1
#define CONFIG_RASPBERRY_TEXT_OFFSET 2
#define CONFIG_QEMU_ARM64_TEXT_OFFSET 3
#if defined(CONFIG_LOAD_OFFSET)
    #if CONFIG_LOAD_OFFSET == CONFIG_ARM64_TEXT_OFFSET
        #define TEXT_OFFSET 0x40080000
    #elif CONFIG_LOAD_OFFSET == CONFIG_RASPBERRY_TEXT_OFFSET
        #define TEXT_OFFSET 0x8000
    #elif CONFIG_LOAD_OFFSET == CONFIG_QEMU_ARM64_TEXT_OFFSET
        #define TEXT_OFFSET 0x80000
    #else 
        #error "Invalid config_value"
    #endif
#else 
    #pragma warning("will use default TEXT_OFFSET") 
    #define TEXT_OFFSET 0x80000
#endif
//自定义调试信息包含内容
#define DWARF_DEBUG \
        .debug  0 :{ *(.debug) } \
        .line   0
#endif