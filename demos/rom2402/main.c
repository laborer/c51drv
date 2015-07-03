/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <rom2402.h>
#include <uart.h>
#include <print.h>


void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
}

void main(void) {
    unsigned char               i;
    unsigned char               c;
    unsigned char __idata       p[16];

    welcome();

    uart_getchar();

    /* Disable write protect */
    P1_0 = 0;

    for (i = 0; i < 16; i++) {
        p[i] = 'A' + i;
    }
    rom2402_writestr(7, 0, p, 16);

    for (i = 2; i < 6; i++) {
        rom2402_write(7, i, 'a' + i);
    }

    for (i = 0; i < 8; i++) {
        c = rom2402_read(7, i);
        UARTCHAR(c);
    }

    UARTCHAR(' ');

    rom2402_readstr(7, 4, p, 8);
    for (i = 0; i < 8; i++) {
        UARTCHAR(p[i]);
    }

    while (1);
}
