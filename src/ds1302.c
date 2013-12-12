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


static void send_byte(unsigned char c)
{
    unsigned char i;

    for (i = 8; i != 0; i--) {
        IO = c & 1;
        DELAY_US(1);
        SCLK = 1;
        DELAY_US(1);
        SCLK = 0;
        c >>= 1;
    }
}

static unsigned char recv_byte(void)
{
    unsigned char c;
    unsigned char i;

    IO = 1;
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

unsigned char ds1302_read(unsigned char addr)
{
    unsigned char c;

    start();
    send_byte((addr << 1) | CMDREAD);
    c = recv_byte();
    stop();

    return c;
}

void ds1302_read_clock(unsigned char __idata *p)
{
    unsigned char i;

    start();
    /* send_byte(0xBF); */
    send_byte((0x1F << 1) | CMDREAD);
    for (i = 7; i != 0; p++, i--) {
        *p = recv_byte();
    }
    stop();
}

void ds1302_write(unsigned char addr, unsigned char c)
{
    start();
    send_byte((addr << 1) | CMDWRITE);
    send_byte(c);
    stop();
}

void ds1302_write_clock(unsigned char __idata *p)
{
    unsigned char i;

    start();
    send_byte((0x1F << 1) | CMDWRITE);
    for (i = 7; i != 0; p++, i--) {
        send_byte(*p);
    }
    send_byte(0);
    stop();
}
