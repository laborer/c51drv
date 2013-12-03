/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "uart.h"
#include "print.h"
#include "timer.h"


#define PUTCHAR(c)                                              \
    do {                                                        \
        uart_putchar(c);                                        \
    } while (0)

#define PUTSTR(str)                                             \
    do {                                                        \
        print_str(uart_putchar, 0, 0, str);                     \
    } while (0)

#define PUTUINT(num)                                            \
    do {                                                        \
        print_int(uart_putchar, PRINT_UNSIGNED, 0, num);        \
    } while (0)


void welcome(void)
{
    uart_baudrate();
    uart_init();
    PUTSTR("c51drv\n");
}

void main(void) {
    unsigned char i;

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

        PUTUINT(t1 - t0);
        PUTCHAR('\n');

        delay_ms(100);
        
        t0 = TIMER0_GET16();
        delay_ms(10);
        t1 = TIMER0_GET16();

        PUTUINT(t1 - t0);
        PUTCHAR('\n');

        delay_ms(100);
        
        t0 = TIMER0_GET16();
        delay_ms(50);
        t1 = TIMER0_GET16();

        PUTUINT(t1 - t0);
        PUTCHAR('\n');
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

        PUTUINT((t1 - t0) >> 16);
        PUTCHAR(' ');
        PUTUINT(t1 - t0);
        PUTCHAR('\n');

        delay_ms(100);
        
        t0 = TIMER0_GET32();
        delay_ms(10);
        t1 = TIMER0_GET32();

        PUTUINT((t1 - t0) >> 16);
        PUTCHAR(' ');
        PUTUINT(t1 - t0);
        PUTCHAR('\n');
    }

    {
        unsigned long t0, t1, t2;

        ET0 = 0;
        t0 = TIMER0_GET32();
        TF0 = 1;
        t1 = TIMER0_GET32();

        PUTCHAR(TF0 ? '1' : '0');
        PUTCHAR('\n');

        ET0 = 1;
        t2 = TIMER0_GET32();

        PUTUINT(t0 >> 16);
        PUTCHAR('.');
        PUTUINT(t0);
        PUTCHAR(' ');
        PUTUINT(t1 >> 16);
        PUTCHAR('.');
        PUTUINT(t1);
        PUTCHAR('\n ');

        PUTCHAR(TF0 ? '1' : '0');
        PUTCHAR('\n');

        PUTUINT(t2 >> 16);
        PUTCHAR('.');
        PUTUINT(t2);
        PUTCHAR('\n ');
    }

    while (1);
}
