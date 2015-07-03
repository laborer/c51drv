/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <uart.h>
#include <irrc5.h>
#include <print.h>


void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
}

static volatile char irstate;
static unsigned int ircode;

void _irrc5_int0(void) __interrupt IE0_VECTOR __using 1
{
    char ret;

    ret = irrc5_falling();
    if (ret <= 0) {
        irstate = ret;
        ircode = irrc5_result();
    } else if (ret == IRRC5_LASTBIT) {
        while (!P3_2);
        irstate = irrc5_rising_last();
        ircode = irrc5_result();
    }
}

void main(void) {
    unsigned char ret;

    welcome();

    irrc5_init();
    P3_2 = 1;
    irstate = 1;
    IT0 = 1;
    EX0 = 1;
    EA = 1;

    while (1) {
        while (irstate > 0);
        if (irstate == 0) {
            irstate = 1;
            UARTHEX4(ircode);
        } else {
            ret = '0' - irstate;
            irstate = 1;
            UARTCHAR('E');
            UARTCHAR(ret);
        }
        UARTCHAR(' ');
    }
}
