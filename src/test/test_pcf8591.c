/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "pcf8591.h"
#include "uart.h"
#include "print.h"


void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
}

void main(void) {
    unsigned char __idata p[4];

    welcome();

    uart_getchar();

    while (1) {
        pcf8591_ctrl(0, 4);
        pcf8591_adcstr(0, p, 4);
        UARTUINT(p[0]);
        UARTCHAR('\n');
        UARTUINT(p[1]);
        UARTCHAR('\n');
        UARTUINT(p[2]);
        UARTCHAR('\n');
        UARTUINT(p[3]);
        UARTCHAR('\n');
        delay_ms(500);
    }

    /* while (1); */
}
