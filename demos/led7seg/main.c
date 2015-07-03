/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <tools.h>
#include <uart.h>
#include <print.h>
#include <led7seg.h>


void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
}

void main(void) {
    unsigned char i;

    welcome();

    P2_7 = 0;
    while (1) {
        for (i = 1; i != 0; i <<= 1) {
            P0 = i;
            UARTHEX2(P0);
            UARTCHAR(' ');
            delay_ms(500);
        }
        delay_ms(1000);

        for (i = 0; i < 16; i++) {
            P0 = led7seg_encode[i];
            UARTHEX2(P0);
            UARTCHAR(' ');
            delay_ms(500);
        }
        delay_ms(1000);
    }
}
