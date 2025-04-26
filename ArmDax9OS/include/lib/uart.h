#pragma once
#include <common/types.h>

// UART初始化
void uart_init(void);

// 发送单个字符
void uart_putc(u32 c);

// 接收单个字符
u32 uart_getc(void);

// 发送字符串
void uart_puts(const char *str);

// 检查发送缓冲区是否为空
int uart_tx_empty(void);

#define NB_UART_NRET ((u32) - 1)

u32 nb_uart_recv(void);
