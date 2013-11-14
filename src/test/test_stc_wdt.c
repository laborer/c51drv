/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "uart.h"
#include "stc/stc89c5xrc_rdp.h"
#include "stc/wdt.h"


static void welcome(void)
{
    uart_baudrate();
    uart_init();
    uart_putstr("c51drv\n");
}

static void _uart_puthex(char c)
{
    c &= 0x0F;
    uart_putchar((c > 9) ? ('A' - 10 + c) : ('0' + c));
}       

void main(void) {
    welcome();

    _uart_puthex(WDT_CONTR >> 4);
    _uart_puthex(WDT_CONTR);

    /* WDT_INIT(0, 1000); */
    /* WDT_ENABLE(); */
    WDT_CONTR = 0x3C;

    while (1) {
        /* WDT_KICK_MS(1000); */
        /* WDT_CONTR |= 0x10; */
        /* if (WDT_CONTR & 0x80) { */
        /*     _uart_puthex(WDT_CONTR >> 4); */
        /*     WDT_CONTR &= ~0x80; */
            /* uart_putchar('x'); */
        /* } else { */
            uart_putchar('.');
        /* } */
        delay_ms(500);
    }
}
