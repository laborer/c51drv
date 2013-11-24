/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "uart.h"
#include "tools.h"
#include "ds1302.h"


void welcome(void)
{
    uart_baudrate();
    uart_init();
    uart_putstr("c51drv\n");
}

void main(void) {
    unsigned char               i;
    unsigned char               j;
    unsigned char __idata       p[7];

    welcome();

    uart_getchar();

    DS1302_WRITE_ENABLE();
    DS1302_CLOCK_RESUME();

    p[0] = uchar2bcd(30);
    p[1] = uchar2bcd(25);
    p[2] = uchar2bcd(10);
    p[3] = uchar2bcd(22);
    p[4] = uchar2bcd(11);
    p[5] = uchar2bcd(6);
    p[6] = uchar2bcd(13);

    ds1302_write_clock(p);
    
    for (i = 90; i != 0; i--) {
        ds1302_read_clock(p);
        for (j = 7; j != 0; j--) {
            uart_putuint(BCD2UCHAR(p[j - 1]));
            uart_putchar(' ');
        }
        uart_putchar('\n');
        delay_ms(1000);
    }

    uart_putstr("clock halt\n");
    DS1302_CLOCK_HALT();
    uart_putstr("write protect\n");
    DS1302_WRITE_PROTECT();

    ds1302_write(DS1302_YEAR, uchar2bcd(99));
    
    for (i = 5; i != 0; i--) {
        ds1302_read_clock(p);
        for (j = 7; j != 0; j--) {
            uart_putuint(BCD2UCHAR(p[j - 1]));
            uart_putchar(' ');
        }
        uart_putchar('\n');
        delay_ms(1000);
    }

    while (1);
}
