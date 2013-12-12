/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
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


static void send_byte(unsigned char c)
{
    RW = 0;
    E = 1;
    DB = c;
    DELAY_US(1);
    E = 0;
}

static unsigned char recv_byte(void)
{
    unsigned char c;

    RW = 1;
    E = 1;
    DELAY_US(1);
    DB = 0xFF;
    c = DB;
    E = 0;

    return c;
}

static void set_addr(unsigned char addr)
{
    WAIT_READY();
    SEND_CMD(0x80 | addr & 0x7F);
}

static unsigned char get_addr(void)
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

void lcd1602_putchar(unsigned char c)
{
    WAIT_READY();
    SEND_DAT(c);
}

unsigned char lcd1602_getchar(void)
{
    WAIT_READY();
    return RECV_DAT();
}

void lcd1602_position(unsigned char x, unsigned char y)
{
    set_addr(x | (y << 6));
}

void lcd1602_write_cgram(unsigned char addr, unsigned char c)
{
    unsigned char t;

    t = get_addr();
    SEND_CMD(0x40 | addr & 0x3F);
    WAIT_READY();
    SEND_DAT(c);
    set_addr(t);
}

unsigned char lcd1602_read_cgram(unsigned char addr)
{
    unsigned char t;
    unsigned char c;

    t = get_addr();
    SEND_CMD(0x40 | addr & 0x3F);
    WAIT_READY();
    c = RECV_DAT();
    set_addr(t);

    return c;
}

void lcd1602_character(unsigned char idx, const unsigned char __code *p)
{
    unsigned char t;
    unsigned char i;

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
