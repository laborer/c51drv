/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <tools.h>
#include <uart.h>
#include <print.h>
#include <timer.h>


void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
}

void main(void) {
    welcome();

    {
        unsigned int t0, t1;
        TIMER0_INIT16();
        TIMER0_SET16(0);
        TIMER0_START();

        delay_ms(100);

        t0 = TIMER0_GET16();
        DELAY_US(10000);
        t1 = TIMER0_GET16();

        UARTUINT(t1 - t0);
        UARTCHAR('\n');

        delay_ms(100);
        
        t0 = TIMER0_GET16();
        delay_ms(10);
        t1 = TIMER0_GET16();

        UARTUINT(t1 - t0);
        UARTCHAR('\n');

        delay_ms(100);
        
        t0 = TIMER0_GET16();
        delay_ms(50);
        t1 = TIMER0_GET16();

        UARTUINT(t1 - t0);
        UARTCHAR('\n');
    }

    {
        unsigned long t0, t1;
        TIMER0_INIT32();
        TIMER0_SET32(0);
        TIMER0_START();

        delay_ms(100);

        t0 = TIMER0_GET32();
        DELAY_US(10000);
        t1 = TIMER0_GET32();

        UARTUINT((t1 - t0) >> 16);
        UARTCHAR(' ');
        UARTUINT(t1 - t0);
        UARTCHAR('\n');

        delay_ms(100);
        
        t0 = TIMER0_GET32();
        delay_ms(10);
        t1 = TIMER0_GET32();

        UARTUINT((t1 - t0) >> 16);
        UARTCHAR(' ');
        UARTUINT(t1 - t0);
        UARTCHAR('\n');
    }

    {
        unsigned long t0, t1, t2;

        ET0 = 0;
        t0 = TIMER0_GET32();
        TF0 = 1;
        t1 = TIMER0_GET32();

        UARTCHAR(TF0 ? '1' : '0');
        UARTCHAR('\n');

        ET0 = 1;
        t2 = TIMER0_GET32();

        UARTUINT(t0 >> 16);
        UARTCHAR('.');
        UARTUINT(t0);
        UARTCHAR(' ');
        UARTUINT(t1 >> 16);
        UARTCHAR('.');
        UARTUINT(t1);
        UARTCHAR('\n ');

        UARTCHAR(TF0 ? '1' : '0');
        UARTCHAR('\n');

        UARTUINT(t2 >> 16);
        UARTCHAR('.');
        UARTUINT(t2);
        UARTCHAR('\n ');
    }

    while (1);
}
