/*  Copyright (c) 2013, laborer (laborer@126.com)
 *  Licensed under the BSD 2-Clause License.
 */


#include "common.h"
#include "rom2402.h"


#define PAGE (1 << ROM2402_PAGE_BITS)

#define SCL ROM2402_SCL
#define SDA ROM2402_SDA


static __bit send_byte(unsigned char c)
{
    unsigned char i;

    SCL = 0;
    DELAY_US(3);

    for (i = 8; i != 0; i--) {
        SDA = c & 0x80;
        SCL = 1;
        DELAY_US(3);
        c <<= 1;
        SCL = 0;
        DELAY_US(3);
    }

    SDA = 1;
    SCL = 1;
    DELAY_US(3);

    return SDA;
}

static unsigned char recv_byte(__bit ack)
{
    unsigned char i;
    unsigned char c;

    c = 0;

    SCL = 0;
    DELAY_US(3);
    SDA = 1;

    for (i = 8; i != 0; i--) {
        c <<= 1;
        SCL = 1;
        DELAY_US(3);
        c |= SDA;
        SCL = 0;
        DELAY_US(3);
    }

    SDA = ack;
    SCL = 1;
    DELAY_US(3);

    return c;
}

static void start()
{
    SCL = 0;
    DELAY_US(3);

    SDA = 1;
    SCL = 1;
    DELAY_US(3);
    SDA = 0;
    DELAY_US(3);
}

static void stop()
{
    SCL = 0;
    DELAY_US(3);
    
    SDA = 0;
    SCL = 1;
    DELAY_US(3);
    SDA = 1;
    DELAY_US(3);
}


void rom2402_reset()
{
    unsigned char i;

    start();

    SCL = 0;
    DELAY_US(3);
    SDA = 1;
        
    for (i = 9; i != 0; i--) {
        SCL = 1;
        DELAY_US(3);
        SCL = 0;
        DELAY_US(3);
    }

    start();
    stop();

    SCL = 0;
    DELAY_US(3);
}

unsigned char rom2402_read(unsigned char dev, unsigned char addr)
{
    unsigned char c;

 start:
    start();
    
    if (send_byte((dev << 1) & 0x0E | 0xA0)) {
        goto start;
    }

    if (send_byte(addr)) {
        goto start;
    }

    start();

    if (send_byte((dev << 1) & 0x0E | 0xA1)) {
        goto start;
    }

    c = recv_byte(1);

    stop();

    return c;
}

void rom2402_readstr(unsigned char dev, 
                     unsigned char addr, 
                     unsigned char __idata *p,
                     unsigned char n)
{
    unsigned char i;

 start:
    start();
    
    if (send_byte((dev << 1) & 0x0E | 0xA0)) {
        goto start;
    }

    if (send_byte(addr)) {
        goto start;
    }

    start();

    if (send_byte((dev << 1) & 0x0E | 0xA1)) {
        goto start;
    }

    i = (~addr & (PAGE - 1)) + 1;
    i = (i > n) ? n : i;
    n -= i;
    addr += i;
    if (addr == 0) {
        dev += 1;
    }

    for (; i != 0; p++, i--) {
        *p = recv_byte(i == 1);
    }

    stop();

    if (n > 0) {
        goto start;
    }
}

void rom2402_write(unsigned char dev, unsigned char addr, unsigned char c)
{
 start:
    start();
    
    if (send_byte((dev << 1) & 0x0E | 0xA0)) {
        goto start;
    }

    if (send_byte(addr)) {
        goto start;
    }

    if (send_byte(c)) {
        goto start;
    }

    stop();
}

void rom2402_writestr(unsigned char dev, 
                      unsigned char addr, 
                      const unsigned char __idata *p,
                      unsigned char n)
{
    unsigned char i;
    unsigned char k;

 start:
    start();
    
    if (send_byte((dev << 1) & 0x0E | 0xA0)) {
        goto start;
    }

    if (send_byte(addr)) {
        goto start;
    }

    i = (~addr & (PAGE - 1)) + 1;
    i = (i > n) ? n : i;
    k = i;

    for (; i != 0; p++, i--) {
        if (send_byte(*p)) {
            p -= k - i;
            goto start;
        }
    }

    n -= k;
    addr += k;
    if (addr == 0) {
        dev += 1;
    }

    stop();

    if (n > 0) {
        goto start;
    }
}
