/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <tools.h>
#include <uart.h>
#include <print.h>
#include <kbhost.h>


void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
}

void main(void) {
    unsigned int ret;

    welcome();

    delay_ms(100);

    kbhost_init();

    while (1) {
        ret = kbhost_read();

        if ((ret & 0xFF) != 0) {
            if (!((unsigned char)(ret >> 8) 
                  & (KBHOST_CONTROL | KBHOST_BREAK | KBHOST_MODIFIER))) {
                kbhost_stop();
                UARTCHAR(ret);
                delay_ms(10);
            }
            /* UARTHEX4(ret); */
            /* UARTCHAR(' '); */
        }
    }
}
