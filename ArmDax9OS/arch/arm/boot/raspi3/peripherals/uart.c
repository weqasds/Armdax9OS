/*
   MIT License

   Copyright (c) 2018 Sergey Matyukevich

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

/*
 * Armdax refers to
 * https://github.com/s-matyukevich/raspberry-pi-os/blob/master/docs/lesson01/rpi-os.md
 * for the min-uart init process.
 */


#include "uart.h"
#include <lib/uart.h>

typedef unsigned int u32;
typedef unsigned long long u64;

void early_uart_init(void)
{
    // 配置GPIO14和GPIO15为ALT5功能
    u32 selector = early_get32(GPFSEL1);
    selector &= ~(7 << 12); // GPIO14
    selector |= 2 << 12;    // ALT5
    selector &= ~(7 << 15); // GPIO15
    selector |= 2 << 15;    // ALT5
    early_put32(GPFSEL1, selector);

    // 禁用上拉/下拉
    early_put32(GPPUD, 0);
    delay(150);
    // 为GPIO14和GPIO15启用时钟
    early_put32(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);
    early_put32(GPPUDCLK0, 0);

    // 启用 mini UART
    early_put32(AUX_ENABLES, 1);
    // 禁用接收/发送
    early_put32(AUX_MU_CNTL_REG, 0);
    // 设置8位数据模式
    early_put32(AUX_MU_LCR_REG, 3);
    // 禁用中断
    early_put32(AUX_MU_IER_REG, 0);
    // 无调试控制
    early_put32(AUX_MU_MCR_REG, 0);
    // 设置波特率 (250MHz / (8*(baud + 1))) 
    early_put32(AUX_MU_BAUD_REG, 270);
    // 启用接收/发送
    early_put32(AUX_MU_CNTL_REG, 3);
}

void early_uart_puts(char *s)
{
    while (*s) {
        switch (*s) {
            case '\n':
                // 等待发送就绪
                while (!(early_get32(AUX_MU_LSR_REG) & 0x20))
                early_put32(AUX_MU_IO_REG, '\r');
                // 继续输出\n
            case '\r':
            case '\t':
                // 等待发送就绪
                while (!(early_get32(AUX_MU_LSR_REG) & 0x20))
                early_put32(AUX_MU_IO_REG, *s++);
                break;
            case '%':
                if (*(s+1) == 'p') {
                    // 输出指针地址
                    u64 val = (u64)*(u64*)(s+2);
                    s += 2;
                    char buf[16];
                    for (int i = 0; i < 16; i++) {
                        buf[15-i] = "0123456789ABCDEF"[val & 0xF];
                        val >>= 4;
                    }
                    for (int i = 0; i < 16; i++) {
                        while (!(early_get32(AUX_MU_LSR_REG) & 0x20));
                        early_put32(AUX_MU_IO_REG, buf[i]);
                    }
                } else if (*(s+1) == 'x') {
                    // 输出16进制数字
                    u32 val = *(u32*)(s+2);
                    s += 2;
                    char buf[8];
                    for (int i = 0; i < 8; i++) {
                        buf[7-i] = "0123456789ABCDEF"[val & 0xF];
                        val >>= 4;
                    }
                    for (int i = 0; i < 8; i++) {
                        while (!(early_get32(AUX_MU_LSR_REG) & 0x20));
                        early_put32(AUX_MU_IO_REG, buf[i]);
                    }
                } else {
                    // 普通%字符
                    while (!(early_get32(AUX_MU_LSR_REG) & 0x20));
                    early_put32(AUX_MU_IO_REG, *s++);
                }
                break;
            default:
                // 等待发送就绪
                while (!(early_get32(AUX_MU_LSR_REG) & 0x20))
                early_put32(AUX_MU_IO_REG, *s++);
        }
    }
}
