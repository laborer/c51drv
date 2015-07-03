/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <uart.h>
#include <print.h>
#include <stc/stc.h>
#include <stc/adc.h>


static void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
}

void main(void) {
    welcome();

    uart_getchar();

    ADC_INIT(1);

    while (1) {
        ADC_START(0, 0);
        DELAY_CYCLES(4);
        UARTSTR("adc: ");
        while (!ADC_FLAG());
        UARTUINT(ADC_RES);
        UARTCHAR('\n');
    }
}
