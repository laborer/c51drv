/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <tools.h>
#include <uart.h>
#include <print.h>
#include <stc/stc.h>
#include <stc/wdt.h>


static void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
}

void main(void) {
    welcome();

    UARTHEX2(WDT_CONTR);

    /* WDT_INIT(0, 1000); */
    /* WDT_ENABLE(); */
    WDT_CONTR = 0x3C;

    while (1) {
        /* WDT_KICK_MS(1000); */
        UARTCHAR('.');
        delay_ms(500);
    }
}
