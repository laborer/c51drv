/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <stc/uart2.h>


void welcome(void)
{
    unsigned char __code *p;

    uart2_baudrate();
    uart2_init();
    for (p = "c51drv\n"; *p != 0; p++) {
        uart2_putchar(*p);
    }
}

void main(void) {
    welcome();

    while (1) {
        uart2_putchar(uart2_getchar());
    }
}
