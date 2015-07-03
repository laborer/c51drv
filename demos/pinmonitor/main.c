/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */

/* Print if a PIN changes */

#include <tools.h>
#include <uart.h>
#include <print.h>


void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
}

void main(void) {
    unsigned char q0;
    unsigned char q1;
    unsigned char q2;
    unsigned char i;
    unsigned char j;
    unsigned char k;

    welcome();

    P0 = 0xFF;
    P1 = 0xFF;
    P2 = 0xFF;
    P3 = 0xFF;

    while (1) {
        q0 = P0;
        q1 = P1;
        q2 = P2;
        delay_ms(250);
        k = q0 ^ P0;
        for (i = 1, j = 0; i > 0; i <<= 1, j++) {
            if (k & i) {
                UARTSTR("P0_");
                UARTCHAR('0' + j);
                UARTCHAR('=');
                UARTCHAR((q0 & i) ? '0' : '1');
                UARTCHAR(' ');
            }
        }
        k = q1 ^ P1;
        for (i = 1, j = 0; i > 0; i <<= 1, j++) {
            if (k & i) {
                UARTSTR("P1_");
                UARTCHAR('0' + j);
                UARTCHAR('=');
                UARTCHAR((q1 & i) ? '0' : '1');
                UARTCHAR(' ');
            }
        }
        k = q2 ^ P2;
        for (i = 1, j = 0; i > 0; i <<= 1, j++) {
            if (k & i) {
                UARTSTR("P2_");
                UARTCHAR('0' + j);
                UARTCHAR('=');
                UARTCHAR((q2 & i) ? '0' : '1');
                UARTCHAR(' ');
            }
        }
    }
}
