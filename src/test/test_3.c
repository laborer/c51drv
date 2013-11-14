/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "uart.h"


void welcome(void)
{
    uart_baudrate();
    uart_init();
    uart_putstr("c51drv\n");
}

void main(void) {
    unsigned char q0;
    unsigned char q1;
    unsigned char q2;
    unsigned char q3;
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
                uart_putstr("P0_");
                uart_putchar('0' + j);
                uart_putchar('=');
                uart_putchar((q0 & i) ? '0' : '1');
                uart_putchar(' ');
            }
        }
        k = q1 ^ P1;
        for (i = 1, j = 0; i > 0; i <<= 1, j++) {
            if (k & i) {
                uart_putstr("P1_");
                uart_putchar('0' + j);
                uart_putchar('=');
                uart_putchar((q1 & i) ? '0' : '1');
                uart_putchar(' ');
            }
        }
        k = q2 ^ P2;
        for (i = 1, j = 0; i > 0; i <<= 1, j++) {
            if (k & i) {
                uart_putstr("P2_");
                uart_putchar('0' + j);
                uart_putchar('=');
                uart_putchar((q2 & i) ? '0' : '1');
                uart_putchar(' ');
            }
        }
    }
}
