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
 * Arm9daxOS refers to
 * https://github.com/s-matyukevich/raspberry-pi-os/blob/master/docs/lesson01/rpi-os.md
 * for the min-uart init process.
 */
#include <common/types.h>
#include <arch/arm/tools.h>
#include <lib/uart.h>
#include "peripherals.h"
/// @brief 延迟指定值时间以ms为模式
/// @param cnt 计数器值 
static inline void delay(unsigned int cnt){
    while(cnt != 0) cnt--;
}

void uart_init(void)
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
    
    //清除屏幕
    clear_screen();
}

/// @brief 清除屏幕输出
static inline void clear_screen(){
    uart_putc(12);
    uart_putc(27);
    uart_putc('[');
    uart_putc('2');
    uart_putc('j');
}

u32 uart_lsr(){
    return get_32(AUX_MU_LSR_REG);
}


u32 uart_getc(void){
    while(1){
        if (uart_lsr() & 0x01)
            break;
    }
    return (get32(AUX_MU_IO_REG) & 0xff);
}

void uart_putc(u32 c){
    while (1) {
        if (uart_lsr() & 0x20)
            break;
    }
put32(AUX_MU_IO_REG, c);
}

void uart_puts(const char *str){}

u32 nb_uart_recv(void){
    if (uart_lsr() & 0x01)
        return (get32(AUX_MU_IO_REG) & 0xFF);
    else
        return NB_UART_NRET;
}
