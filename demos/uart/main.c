/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <uart.h>


void welcome(void)
{
    unsigned char __code *p;

    while (uart_baudrate_auto());
    uart_init();
    for (p = "c51drv\n"; *p != 0; p++) {
        uart_putchar(*p);
    }
}

void main(void) {
    unsigned char c;
    
    welcome();

    while (1) {
        c = uart_getchar();
        uart_putchar(' ');
        uart_putchar(c);
        uart_putchar('(');
    }
}
