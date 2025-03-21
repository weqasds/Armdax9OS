#ifndef ASM_H
#define ASM_H
//自动生成汇编函数头
#define ASM_FUNC_BEGIN(_name)             \
        .global _name;                    \
        .type _name, "function";          \
        _name:
//自动生成汇编函数尾部
#define ASM_FUNC_END(_name) .size _name, .- _name 
#endif

