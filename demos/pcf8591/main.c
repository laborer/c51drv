/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <tools.h>
#include <uart.h>
#include <print.h>
#include <pcf8591.h>


void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
}

void main(void) {
    unsigned char __idata       p[5];
    unsigned char               ctrl;

    welcome();

    uart_getchar();

    ctrl = PCF8591_OUTPUTENABLE | PCF8591_INPUTCONFIG0 | PCF8591_AUTOINCREMENT;
    pcf8591_ctrl(0, ctrl);

    while (1) {
        pcf8591_adcstr(0, p, 5);
        pcf8591_dac(0, ctrl, p[1]);

        UARTUINT(p[1]);
        UARTCHAR(' ');
        UARTUINT(p[2]);
        UARTCHAR(' ');
        UARTUINT(p[3]);
        UARTCHAR(' ');
        UARTUINT(p[4]);
        UARTCHAR('\n');

        delay_ms(500);
    }
}
