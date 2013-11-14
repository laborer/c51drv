/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "uart.h"
#include "irnec.h"


void welcome(void)
{
    uart_baudrate();
    uart_init();
    uart_putstr("c51drv\n");
}

static volatile char irstate;
static unsigned int ircode;

void _irnec_int0(void) __interrupt IE0_VECTOR __using 1
{
    char ret;

    ret = irnec_falling();
    if (ret <= 0) {
        irstate = ret;
        ircode = irnec_result();
    }
}

void _uart_puthex(char c)
{
    c &= 0x0F;
    uart_putchar((c > 9) ? ('A' - 10 + c) : ('0' + c));
}       

void main(void) {
    unsigned char ret;

    welcome();

    irnec_init();
    P3_2 = 1;
    irstate = 1;
    IT0 = 1;
    EX0 = 1;
    EA = 1;

    while (1) {
        while (irstate > 0);
        if (irstate == 0) {
            irstate = 1;
            _uart_puthex(ircode >> 12);
            _uart_puthex(ircode >> 8);
            _uart_puthex(ircode >> 4);
            _uart_puthex(ircode);
        } else {
            ret = '0' - irstate;
            irstate = 1;
            uart_putchar('E');
            uart_putchar(ret);
        }
        uart_putchar(' ');
    }
}


/* void main(void) { */
/*     char ret; */

/*     welcome(); */

/*     irnec_init(); */

/*     while (1) { */
/*         ret = irnec_polling(); */
/*         if (ret == IRNEC_ERR_REPEAT) { */
/*             uart_putchar('R'); */
/*         } else if (ret < 0) { */
/*             uart_putchar('E'); */
/*         } else if (ret == 0) { */
/*             uart_putchar('D'); */
/*             uart_putuint(irnec_result()); */
/*             uart_putchar('\n'); */
/*         } */
/*     } */
/* } */
