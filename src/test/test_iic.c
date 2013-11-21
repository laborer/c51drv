/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "iic.h"
#include "uart.h"


void welcome(void)
{
    uart_baudrate();
    uart_init();
    uart_putstr("c51drv\n");
}

void main(void) {
    welcome();

    while (1);
}
