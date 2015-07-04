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

#define SEND_BYTE(rs, c)                                        \
    do {                                                        \
        E = 0;                                                  \
        DELAY_US(1);                                            \
        RS = rs;                                                \
        RW = 0;                                                 \
        E = 1;                                                  \
        DB = (c);                                               \
        DELAY_US(1);                                            \
        E = 0;                                                  \
    } while (0)

#define RECV_BYTE(rs)                                           \
    do {                                                        \
        E = 0;                                                  \
        DELAY_US(1);                                            \
        RS = rs;                                                \
        RW = 1;                                                 \
        E = 1;                                                  \
        DELAY_US(1);                                            \
        DB = 0xFF;                                              \
    } while (DB & 0x80)


uint8_t lcd1602_getaddr(void)
{
    RECV_BYTE(0);
    RECV_BYTE(0);
    return DB;
}

void lcd1602_cmd(uint8_t cmd)
{
    RECV_BYTE(0);
    SEND_BYTE(0, cmd);
}

uint8_t lcd1602_getchar()
{
    RECV_BYTE(0);
    RECV_BYTE(1);
    return DB;
}

void lcd1602_putchar(uint8_t c)
{
    RECV_BYTE(0);
    SEND_BYTE(1, c);
}

void lcd1602_character(uint8_t idx, const uint8_t __code *p)
{
    uint8_t     addr;
    uint8_t     i;

    addr = lcd1602_getaddr();

    lcd1602_cmd(0x40 | (idx << 3) & 0x3F);
    for (i = 8; i != 0; i--, p++) {
        lcd1602_putchar(*p);
    }
    LCD1602_SETADDR(addr);
}

void lcd1602_init(void)
{
    uint8_t i;

    E = 0;
    RS = 0;
    delay_ms(15);

    for (i = 3; i != 0; i--) {
        SEND_BYTE(0, 0x38);
        delay_ms(5);
    }

    LCD1602_SWITCH(0, 0, 0);
    LCD1602_CLEAR();
    LCD1602_ENTRY(1, 0);
    LCD1602_HOME();
    LCD1602_SWITCH(1, 0, 0);
}
