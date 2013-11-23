/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "uart.h"
#include "timer.h"
#include "stc/stc89c5xrc_rdp.h"


void welcome(void)
{
    uart_baudrate();
    uart_init();
    uart_putstr("c51drv\n");
}

void main(void) {
    unsigned char i;

    welcome();

    /* TIMER0_COUNTDOWN_US(10000); */
    /* i = 0; */
    /* while (1) { */
    /*     if (TIMER0_FLAG) { */
    /*         uart_putint(i); */
    /*         uart_putchar(' '); */
    /*         TIMER0_COUNTDOWN_US(10000); */
    /*         i = 0; */
    /*     } */
    /*     i++; */
    /*     DELAY_US(1000); */
    /* } */


    /* timer2_set16(-10000); */
    /* TIMER2_START(); */
    /* i = 0; */
    /* while (1) { */
    /*     if (TIMER2_FLAG) { */
    /*         TIMER2_FLAG = 0; */
    /*         uart_putint(i); */
    /*         uart_putchar(' '); */
    /*         i = 0; */
    /*     } */
    /*     i++; */
    /*     DELAY_US(1000); */
    /* } */


    {
        unsigned int t0, t1;
        TIMER0_INIT16();
        TIMER0_SET16(0);
        TIMER0_START();

        delay_ms(100);

        t0 = TIMER0_GET16();
        DELAY_US(10000);
        t1 = TIMER0_GET16();

        uart_putuint(t1 - t0);
        uart_putchar('\n');

        delay_ms(100);
        
        t0 = TIMER0_GET16();
        delay_ms(10);
        t1 = TIMER0_GET16();

        uart_putuint(t1 - t0);
        uart_putchar('\n');

        delay_ms(100);
        
        t0 = TIMER0_GET16();
        delay_ms(50);
        t1 = TIMER0_GET16();

        uart_putuint(t1 - t0);
        uart_putchar('\n');
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

        uart_putuint((t1 - t0) >> 16);
        uart_putchar(' ');
        uart_putuint(t1 - t0);
        uart_putchar('\n');

        delay_ms(100);
        
        t0 = TIMER0_GET32();
        delay_ms(10);
        t1 = TIMER0_GET32();

        uart_putuint((t1 - t0) >> 16);
        uart_putchar(' ');
        uart_putuint(t1 - t0);
        uart_putchar('\n');
    }

    {
        unsigned long t0, t1, t2;

        ET0 = 0;
        t0 = TIMER0_GET32();
        TF0 = 1;
        t1 = TIMER0_GET32();

        uart_putchar(TF0 ? '1' : '0');
        uart_putchar('\n');

        ET0 = 1;
        t2 = TIMER0_GET32();

        uart_putuint(t0 >> 16);
        uart_putchar('.');
        uart_putuint(t0);
        uart_putchar(' ');
        uart_putuint(t1 >> 16);
        uart_putchar('.');
        uart_putuint(t1);
        uart_putchar('\n ');

        /* uart_putuint((t1 - t0) >> 16); */
        /* uart_putchar(' '); */
        /* uart_putuint(t1 - t0); */
        /* uart_putchar('\n'); */

        uart_putchar(TF0 ? '1' : '0');
        uart_putchar('\n');

        uart_putuint(t2 >> 16);
        uart_putchar('.');
        uart_putuint(t2);
        uart_putchar('\n ');
    }

    while (1);

    {
        unsigned long t0, t1;
        TIMER0_INIT32();
        TIMER0_START();
        t0 = TIMER0_GET32();
        DELAY_US(10000);
        //delay_ms(1000);
        t1 = TIMER0_GET32();
        uart_putuint((t1 - t0) >> 16);
        uart_putchar(' ');
        uart_putuint(t1 - t0);

        while (1);
    }

    for (i = 0; i < 1000; i++) {
        unsigned long t = TIMER0_GET32();
        uart_putuint(t >> 16);
        uart_putchar(' ');
        uart_putuint(t);
        uart_putchar('\n');
        /* DELAY_US(1000); */
    }
}
