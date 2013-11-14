/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "tools.h"
#include "rom2402.h"
#include "uart.h"


void welcome(void)
{
    uart_baudrate();
    uart_init();
    uart_putstr("c51drv\n");
}

void main(void) {
    unsigned char               i;
    unsigned char               c;
    unsigned char __idata       p[16];

    welcome();

    for (i = 0; i < 16; i++) {
        p[i] = 'a' + i;
    }
    rom2402_writestr(0, 0, p, 16);

    for (i = 2; i < 6; i++) {
        rom2402_write(0, i, 'A' + i);
    }
    
    for (i = 0; i < 8; i++) {
        c = rom2402_read(0, i);
        uart_putchar(c);
    }

    rom2402_reset();
    uart_putchar(' ');

    rom2402_readstr(0, 4, p, 8);
    for (i = 0; i < 8; i++) {
        uart_putchar(p[i]);
    }

    while (1);
}
