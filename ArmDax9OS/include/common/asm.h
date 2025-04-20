#pragma once
//自动生成汇编函数头
#define ASM_FUNC_BEGIN(_name)             \
        .global _name;                    \
        .type _name, "function";          \
        _name:
//自动生成汇编函数尾部
#define ASM_FUNC_END(_name) .size _name, .- _name 

#define EXPORT(symbol) \
        .globl symbol; \
        symbol:
        
#define LOCAL_DATA(x) \
        .type x, 1;   \
        x:

#define DATA(x)    \
        .global x; \
        .hidden x; \
        LOCAL_DATA(x)

#define END_DATA(x) .size x, .- x


