/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "tools.h"
#include "lcd1602.h"


#define RS      LCD1602_RS
#define RW      LCD1602_RW
#define E       LCD1602_E
#define DB      LCD1602_DB


#define SEND_CMD(c)                                             \
    do {                                                        \
        RS = 0;                                                 \
        send_byte(c);                                           \
    } while (0)

#define SEND_DAT(c)                                             \
    do {                                                        \
        RS = 1;                                                 \
        send_byte(c);                                           \
    } while (0)

#define RECV_CMD()                                              \
    (RS = 0, recv_byte())

#define RECV_DAT()                                              \
    (RS = 1, recv_byte())

#define WAIT_READY()                                            \
    while (RECV_CMD() & 0x80) {}


static void send_byte(uint8_t c)
{
    RW = 0;
    E = 1;
    DB = c;
    DELAY_US(1);
    E = 0;
}

static uint8_t recv_byte(void)
{
    uint8_t c;

    RW = 1;
    E = 1;
    DELAY_US(1);
    DB = 0xFF;
    c = DB;
    E = 0;

    return c;
}

static void set_addr(uint8_t addr)
{
    WAIT_READY();
    SEND_CMD(0x80 | addr & 0x7F);
}

static uint8_t get_addr(void)
{
    WAIT_READY();
    return RECV_CMD() & 0x7F;
}

void lcd1602_clear_display(void)
{
    WAIT_READY();
    SEND_CMD(0x01);
}

void lcd1602_return_home(void)
{
    WAIT_READY();
    SEND_CMD(0x02);
}

void lcd1602_entry_mode(__bit direction, __bit shift)
{
    WAIT_READY();
    SEND_CMD(0x04 | (direction << 1) | shift);
}

void lcd1602_switch(__bit display, __bit cursor, __bit blink)
{
    WAIT_READY();
    SEND_CMD(0x08 | (display << 2) | (cursor << 1) | blink);
}

void lcd1602_shift(__bit display, __bit direction)
{
    WAIT_READY();
    SEND_CMD(0x10 | (display << 3) | (direction << 2));
}

void lcd1602_putchar(uint8_t c)
{
    WAIT_READY();
    SEND_DAT(c);
}

uint8_t lcd1602_getchar(void)
{
    WAIT_READY();
    return RECV_DAT();
}

void lcd1602_position(uint8_t x, uint8_t y)
{
    set_addr(x | (y << 6));
}

void lcd1602_write_cgram(uint8_t addr, uint8_t c)
{
    uint8_t t;

    t = get_addr();
    SEND_CMD(0x40 | addr & 0x3F);
    WAIT_READY();
    SEND_DAT(c);
    set_addr(t);
}

uint8_t lcd1602_read_cgram(uint8_t addr)
{
    uint8_t     t;
    uint8_t     c;

    t = get_addr();
    SEND_CMD(0x40 | addr & 0x3F);
    WAIT_READY();
    c = RECV_DAT();
    set_addr(t);

    return c;
}

void lcd1602_character(uint8_t idx, const uint8_t __code *p)
{
    uint8_t     t;
    uint8_t     i;

    t = get_addr();
    for (i = 0; i < 8; i++, p++) {
        WAIT_READY();
        SEND_CMD(0x40 | i | (idx << 3) & 0x3F);
        WAIT_READY();
        SEND_DAT(*p);
    }
    set_addr(t);
}

void lcd1602_init(void)
{
    E = 0;
    RS = 0;
    delay_ms(15);

    SEND_CMD(0x38);
    delay_ms(5);
    SEND_CMD(0x38);
    delay_ms(5);
    SEND_CMD(0x38);
    delay_ms(5);
    SEND_CMD(0x38);

    lcd1602_switch(0, 0, 0);
    lcd1602_clear_display();
    lcd1602_entry_mode(1, 0);
    lcd1602_return_home();
    lcd1602_switch(1, 0, 0);
}
