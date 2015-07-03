/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <uart.h>
#include <print.h>
#include <stc/stc.h>
#include <stc/gpio.h>


static void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
}

void main(void) {
    welcome();

    GPIO_PINMODE(0, 1, GPIO_OUT);

    GPIO_PORTMODE(1, GPIO_IN);

    while (1);
}
