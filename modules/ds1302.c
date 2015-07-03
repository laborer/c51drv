/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "ds1302.h"


#define CE              DS1302_CE
#define SCLK            DS1302_SCLK
#define IO              DS1302_IO

#define CMDREAD         0x81
#define CMDWRITE        0x80


static void send_byte(uint8_t c)
{
    uint8_t i;

    for (i = 8; i != 0; i--) {
        IO = c & 1;
        DELAY_US(1);
        SCLK = 1;
        DELAY_US(1);
        SCLK = 0;
        c >>= 1;
    }
}

static uint8_t recv_byte(void)
{
    uint8_t     c;
    uint8_t     i;

    IO = 1;
    c = 0;
    for (i = 8; i != 0; i--) {
        c >>= 1;
        DELAY_US(1);
        if (IO) {
            c |= 0x80;
        }
        SCLK = 1;
        DELAY_US(1);
        SCLK = 0;
    }

    return c;
}

static void start(void)
{
    CE = 0;
    SCLK = 0;
    DELAY_US(1);

    CE = 1;
    DELAY_US(3);
}

static void stop(void)
{
    CE = 0;
}

uint8_t ds1302_read(uint8_t addr)
{
    uint8_t c;

    start();
    send_byte((addr << 1) | CMDREAD);
    c = recv_byte();
    stop();

    return c;
}

void ds1302_read_clock(uint8_t __idata *p)
{
    uint8_t i;

    start();
    /* send_byte(0xBF); */
    send_byte((0x1F << 1) | CMDREAD);
    for (i = 7; i != 0; p++, i--) {
        *p = recv_byte();
    }
    stop();
}

void ds1302_write(uint8_t addr, uint8_t c)
{
    start();
    send_byte((addr << 1) | CMDWRITE);
    send_byte(c);
    stop();
}

void ds1302_write_clock(uint8_t __idata *p)
{
    uint8_t i;

    start();
    send_byte((0x1F << 1) | CMDWRITE);
    for (i = 7; i != 0; p++, i--) {
        send_byte(*p);
    }
    send_byte(0);
    stop();
}
