/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "uart.h"


void welcome(void)
{
    while (uart_baudrate_auto());
    uart_init();
    uart_putstr("c51drv\n");
}

void main(void) {
    unsigned char c;
    
    welcome();

    while (1) {
        c = uart_getchar();
        uart_putchar(' ');
        uart_putchar(c);
        uart_putchar('(');
        uart_putint(c);
        uart_putchar(')');
    }
}
