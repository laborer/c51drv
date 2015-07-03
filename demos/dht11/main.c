/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <tools.h>
#include <uart.h>
#include <print.h>
#include <dht11.h>


void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
}

void main(void) {
    unsigned char __idata       out[4];
    unsigned char               i;

    welcome();

    while (1) {
        UARTHEX2(dht11_read(out));
        for (i = 0; i < 4; i++) {
            UARTCHAR(' ');
            UARTUINT(out[i]);
        }
        UARTCHAR('\n');
        delay_ms(1000);
    }
}
