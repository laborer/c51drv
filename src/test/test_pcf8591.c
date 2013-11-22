/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "pcf8591.h"
#include "uart.h"


void welcome(void)
{
    uart_baudrate();
    uart_init();
    uart_putstr("c51drv\n");
}

void main(void) {
    unsigned char __idata p[4];

    welcome();

    uart_getchar();

    while (1) {
        pcf8591_ctrl(0, 4);
        pcf8591_adcstr(0, p, 4);
        uart_putuint(p[0]);
        uart_putchar('\n');
        uart_putuint(p[1]);
        uart_putchar('\n');
        uart_putuint(p[2]);
        uart_putchar('\n');
        uart_putuint(p[3]);
        uart_putchar('\n');
        delay_ms(500);
    }

    /* while (1); */
}
