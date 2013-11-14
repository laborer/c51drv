/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "uart.h"
#include "ds1820.h"


void welcome(void)
{
    uart_baudrate();
    uart_init();
    uart_putstr("c51drv\n");
}

void main(void) {
    unsigned char __idata       rom[8];
    unsigned char               i;
    int                         t;
    unsigned char __idata       a;
    unsigned char __idata       b;

    welcome();

    rom[0] = 0;
    rom[1] = 0;
    rom[2] = 0;
    rom[3] = 0;
    rom[4] = 0;
    rom[5] = 0;
    rom[6] = 0;
    rom[7] = 0;

    uart_putchar('0' + ds1820_search_rom(rom));

    for (i = 0; i < 8; i++) {
        uart_putchar(rom[i]);
    }

    /* uart_putchar('0' + ds1820_search_rom(rom)); */

    /* for (i = 0; i < 8; i++) { */
    /*     uart_putchar(rom[i]); */
    /* } */

    
    uart_putstr(" P");
    uart_putchar('0' + ds1820_read_power_supply(0));
    uart_putchar('0' + ds1820_read_power_supply(rom));

    uart_putstr(" R");
    uart_putchar('0' + ds1820_write_resolution(rom, 10));
    uart_putchar('0' + ds1820_read_resolution(rom) - 9);

    uart_putstr(" T");
    uart_putchar('0' + ds1820_write_triggers(rom, 'c', 'd'));
    uart_putchar('0' + ds1820_read_triggers(rom, &a, &b));
    uart_putchar(a);
    uart_putchar(b);

    /* uart_putchar('C'); */

    /* uart_putchar('0' + ds1820_recall_e2(rom)); */

    /* delay_ms(100); */

    /* uart_putchar('0' + ds1820_read_resolution(rom) - 9); */

    /* uart_putchar('0' + ds1820_read_triggers(rom, &buf[0], &buf[1])); */
    
    /* uart_putchar(buf[0]); */
    /* uart_putchar(buf[1]); */

    /* uart_putchar('W'); */

    /* uart_putint(ds1820_read_temperature(rom) / 16); */

    /* uart_putchar('0' + ds1820_read_temperature(0)); */
    
    uart_putstr(" W");
    uart_putchar('0' + ds1820_write_resolution(0, 12));
    t = ds1820_measure_temperature(0);
    if (((unsigned int)t >> 8) == 0x80) {
        uart_putchar('0' + (t & 0xFF));
    } else {
        /* uart_putint(t / 16); */
        uart_putint(t >> 4);
    }

    uart_putstr(" W");
    uart_putchar('0' + ds1820_write_resolution(0, 9));
    t = ds1820_measure_temperature(0);
    if (((unsigned int)t >> 8) == 0x80) {
        uart_putchar('0' + (t & 0xFF));
    } else {
        /* uart_putint(t / 16); */
        uart_putint(t >> 4);
    }

    while (1);
}
