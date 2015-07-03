/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <tools.h>
#include <uart.h>
#include <print.h>
#include <timer.h>
#include <hcsr04.h>


void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
}

void main(void) {
    unsigned int d;

    welcome();

    while (1) {
        d = (unsigned long)hcsr04_read() * 17 / TIMER_CYCLES_US(1000);

        UARTUINT(d);
        UARTSTR("cm\n");
            
        delay_ms(500);
    }
}
