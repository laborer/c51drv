/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "uart.h"
#include "timer.h"


unsigned int __xdata t[80];


void welcome(void)
{
    uart_baudrate();
    uart_init();
    uart_putstr("c51drv\n");
}

void main(void) {
    char i;
    __bit b;

    TIMER0_INIT16();
    TIMER0_START();

    P3_0 = 1;
    b = 1;
    
    for (i = 0; i < 80; i++) {
        while (b == P3_0);
        t[i] = TIMER0_GET16();
        b = !b;
    }

    while (1) {
        welcome();

        for (i = 1; i < 80; i++) {
            uart_putuint(i);
            uart_putstr(" +");
            uart_putuint(t[i] - t[i - 1]);
            uart_putchar('\n');
        }

        delay_ms(5000);
    }
}
