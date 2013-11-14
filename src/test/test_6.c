/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "uart.h"


void welcome(void)
{
    uart_baudrate();
    uart_init();
    uart_putstr("c51drv\n");
}

void main(void) {
    unsigned char __code *p;

    welcome();
    
    uart_getchar();

    for (p = 0; p < 8 * 1024; p++) {
        uart_putchar(*p);
    }

    while (1);
}
