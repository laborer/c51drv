/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <uart.h>
#include <ds1820.h>
#include <print.h>


void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
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

    UARTCHAR('0' + ds1820_search_rom(rom));

    for (i = 0; i < 8; i++) {
        UARTCHAR(rom[i]);
    }

    /* UARTCHAR('0' + ds1820_search_rom(rom)); */

    /* for (i = 0; i < 8; i++) { */
    /*     UARTCHAR(rom[i]); */
    /* } */

    
    UARTSTR(" P");
    UARTCHAR('0' + ds1820_read_power_supply(0));
    UARTCHAR('0' + ds1820_read_power_supply(rom));

    UARTSTR(" R");
    UARTCHAR('0' + ds1820_write_resolution(rom, 10));
    UARTCHAR('0' + ds1820_read_resolution(rom) - 9);

    UARTSTR(" T");
    UARTCHAR('0' + ds1820_write_triggers(rom, 'c', 'd'));
    UARTCHAR('0' + ds1820_read_triggers(rom, &a, &b));
    UARTCHAR(a);
    UARTCHAR(b);

    /* UARTCHAR('C'); */

    /* UARTCHAR('0' + ds1820_recall_e2(rom)); */

    /* delay_ms(100); */

    /* UARTCHAR('0' + ds1820_read_resolution(rom) - 9); */

    /* UARTCHAR('0' + ds1820_read_triggers(rom, &buf[0], &buf[1])); */
    
    /* UARTCHAR(buf[0]); */
    /* UARTCHAR(buf[1]); */

    /* UARTCHAR('W'); */

    /* UARTINT(ds1820_read_temperature(rom) / 16); */

    /* UARTCHAR('0' + ds1820_read_temperature(0)); */
    
    UARTSTR(" W");
    UARTCHAR('0' + ds1820_write_resolution(0, 12));
    t = ds1820_measure_temperature(0);
    if (((unsigned int)t >> 8) == 0x80) {
        UARTCHAR('0' + (t & 0xFF));
    } else {
        /* UARTINT(t / 16); */
        UARTINT(t >> 4);
    }

    UARTSTR(" W");
    UARTCHAR('0' + ds1820_write_resolution(0, 9));
    t = ds1820_measure_temperature(0);
    if (((unsigned int)t >> 8) == 0x80) {
        UARTCHAR('0' + (t & 0xFF));
    } else {
        /* UARTINT(t / 16); */
        UARTINT(t >> 4);
    }

    while (1);
}
