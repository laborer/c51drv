/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include <tools.h>
#include <uart.h>
#include <lcd1602.h>
#include <print.h>


void welcome(void)
{
    uart_baudrate();
    uart_init();
    UARTSTR("c51drv\n");
}

void main(void) {
    unsigned char i;
    unsigned char __code chn_year[] = {
        BIN(01000),
        BIN(01111),
        BIN(10010),
        BIN(01111),
        BIN(01010),
        BIN(11111),
        BIN(00010),
        BIN(00000)
    };
    unsigned char __code chn_month[] = {
        BIN(01111),
        BIN(01001),
        BIN(01111),
        BIN(01001),
        BIN(01111),
        BIN(01001),
        BIN(10011),
        BIN(00000)
    };
    unsigned char __code chn_day[] = {
        BIN(11111),
        BIN(10001),
        BIN(10001),
        BIN(11111),
        BIN(10001),
        BIN(10001),
        BIN(11111),
        BIN(00000)
    };
    unsigned char __code *p;

    welcome();

    lcd1602_init();

    /* lcd1602_entry_mode(1, 1); */
    LCD1602_SWITCH(1, 1, 1);
    /* lcd1602_switch(1, 1, 1); */

    LCD1602_POSITION(0, 0);

    for (i = 'A'; i <= 'Z'; i++) {
        lcd1602_putchar(i);
    }

    LCD1602_POSITION(8, 1);

    for (i = '0'; i <= '9'; i++) {
        lcd1602_putchar(i);
    }

    /* lcd1602_write_cgram(0, 'A'); */

    /* UARTCHAR(lcd1602_read_cgram(0)); */

    LCD1602_POSITION(0, 1);

    lcd1602_character(0, chn_year);
    lcd1602_character(1, chn_month);
    lcd1602_character(2, chn_day);

    for (p = "2013"; *p != 0; p++) {
        lcd1602_putchar(*p);
    }
    lcd1602_putchar(0);
    for (p = "10"; *p != 0; p++) {
        lcd1602_putchar(*p);
    }
    lcd1602_putchar(1);
    for (p = "10"; *p != 0; p++) {
        lcd1602_putchar(*p);
    }
    lcd1602_putchar(2);

    LCD1602_HOME();

    while (i = uart_getchar()) {
        lcd1602_putchar(i);
    }

    while (1);
}

