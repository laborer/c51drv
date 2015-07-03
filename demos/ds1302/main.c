/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <uart.h>
#include <print.h>
#include <tools.h>
#include <ds1302.h>


void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
}

void main(void) {
    unsigned char               i;
    unsigned char               j;
    unsigned char __idata       p[7];

    welcome();

    uart_getchar();

    DS1302_WRITEPROTECT_DISABLE();
    DS1302_CLOCKHALT_DISABLE();

    p[0] = uchar2packedbcd(30);
    p[1] = uchar2packedbcd(25);
    p[2] = uchar2packedbcd(10);
    p[3] = uchar2packedbcd(22);
    p[4] = uchar2packedbcd(11);
    p[5] = uchar2packedbcd(6);
    p[6] = uchar2packedbcd(13);

    ds1302_write_clock(p);
    
    for (i = 90; i != 0; i--) {
        ds1302_read_clock(p);
        for (j = 7; j != 0; j--) {
            UARTUINT(PACKEDBCD2UCHAR(p[j - 1]));
            UARTCHAR(' ');
        }
        UARTCHAR('\n');
        delay_ms(1000);
    }

    UARTSTR("clock halt\n");
    DS1302_CLOCKHALT_ENABLE();
    UARTSTR("write protect\n");
    DS1302_WRITEPROTECT_ENABLE();

    ds1302_write(DS1302_YEAR, uchar2packedbcd(99));
    
    for (i = 5; i != 0; i--) {
        ds1302_read_clock(p);
        for (j = 7; j != 0; j--) {
            UARTUINT(PACKEDBCD2UCHAR(p[j - 1]));
            UARTCHAR(' ');
        }
        UARTCHAR('\n');
        delay_ms(1000);
    }

    while (1);
}
